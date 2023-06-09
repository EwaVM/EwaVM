#include <internal/def.h>

namespace EwaVM
{
  namespace OpGen
  {
    void opgen_GenRefNull(ModuleCompiler *m, int32_t typeidx)
    {
      StackValue *sv = stackvalue_Push(m, WVT_REF);
      sv->jit_type = SVT_GENERAL;
      sv->val.op = SLJIT_IMM;
      sv->val.opw = 0;
    }
    void opgen_GenIsNull(ModuleCompiler *m)
    {
      StackValue *sv = &m->stack[m->sp];
      if (sv->wasm_type == WVT_REF || sv->wasm_type == WVT_FUNC)
      {
        sljit_emit_op2u(m->jitc, SLJIT_SUB | SLJIT_SET_Z, sv->val.op, sv->val.opw,
                        SLJIT_IMM, 0);
        m->sp--;
        stackvalue_Push(m, WVT_I32);
        sv->jit_type = SVT_CMP;
        sv->val.cmp.flag = SLJIT_EQUAL;
      }
      else
      {
        m->sp--;
        stackvalue_Push(m, WVT_I32);
        sv->jit_type = SVT_GENERAL;
        sv->val.opw = 0;
        sv->val.op = SLJIT_IMM;
      }
    }

    void opgen_GenRefFunc(ModuleCompiler *m, int32_t fidx)
    {
      int32_t a;
      StackValue *sv;
      a = GetFreeReg(m, RT_INTEGER, 0);
      sljit_emit_op1(m->jitc, SLJIT_MOV, a, 0, SLJIT_IMM, (sljit_uw)(m->context->funcentries + fidx));
      sv = stackvalue_Push(m, WVT_FUNC);
      sv->jit_type = SVT_GENERAL;
      sv->val.op = SLJIT_MEM1(a);
      sv->val.opw = 0;
    }

    void opgen_GenTableSize(ModuleCompiler *m, int tabidx)
    {
      int32_t r;
      StackValue *sv = NULL;
      r = GetFreeReg(m, RT_INTEGER, 0);
      Table *tab = *dynarr_get(m->context->tables, Table *, tabidx);
      sljit_emit_op1(m->jitc, SLJIT_MOV, r, 0, SLJIT_IMM, (sljit_sw)&tab->size);
      sv = stackvalue_Push(m, WVT_I32);
      sv->jit_type = SVT_GENERAL;
      sv->val.op = SLJIT_MEM1(r);
      sv->val.opw = 0;
    }

    char *opgen_GenMiscOp_FC(ModuleCompiler *m, int opcode)
    {
      StackValue *sv;
      int a, b;
#if DEBUG_BUILD
      switch (opcode)
      {
      case 0 ... 7:
        wa_debug("op fc %x:%s\n", m->pc, "ixx.trunc_sat_fxx_s|u");
        break;
      case 0x0a:
        wa_debug("op fc %x:%s\n", m->pc, "memory.copy");
        break;
      case 0x0b:
        wa_debug("op fc %x:%s\n", m->pc, "memory.fill");
        break;
      }
#endif
      switch (opcode)
      {
      // Non-trapping Float-to-int Conversions
      // the behavior when convert is overflow is depend on sljit implementation.
      case 0x00: // i32.trunc_sat_f32_s
        opgen_GenNumOp(m, 0xa8);
        break;
      case 0x01: // i32.trunc_sat_f32_u
        opgen_GenNumOp(m, 0xa9);
        break;
      case 0x02: // i32.trunc_sat_f64_s
        opgen_GenNumOp(m, 0xaa);
        break;
      case 0x03: // i32.trunc_sat_f64_u
        opgen_GenNumOp(m, 0xab);
        break;
      case 0x04: // i64.trunc_sat_f32_s
        opgen_GenNumOp(m, 0xae);
        break;
      case 0x05: // i64.trunc_sat_f32_u
        opgen_GenNumOp(m, 0xaf);
        break;
      case 0x06: // i64.trunc_sat_f64_s
        opgen_GenNumOp(m, 0xb0);
        break;
      case 0x07: // i64.trunc_sat_f64_u
        opgen_GenNumOp(m, 0xb1);
        break;
      case 0x0a:                                               // memory.copy
        a = read_LEB_signed(m->bytes, (uint32_t *)&m->pc, 32); // destination memory index
        b = read_LEB_signed(m->bytes, (uint32_t *)&m->pc, 32); // source memory index
        opgen_GenMemoryCopy(m, a, b);
        break;
      case 0x0b:                                               // memory.fill
        a = read_LEB_signed(m->bytes, (uint32_t *)&m->pc, 32); // destination memory index
        opgen_GenMemoryFill(m, a);
        break;
      case 0x0e: // table.copy
      {
        int32_t dtab = read_LEB_signed(m->bytes, (uint32_t *)&m->pc, 32); // destination table index
        int32_t stab = read_LEB_signed(m->bytes, (uint32_t *)&m->pc, 32); // source table index
        opgen_GenTableCopy(m, dtab, stab);
      }
      break;
      case 0x0f: // table.grow
        // table are not allowed to grow
        read_LEB_signed(m->bytes, (uint32_t *)&m->pc, 32); // table index
        m->sp -= 2;
        sv = stackvalue_Push(m, WVT_I32);
        sv->val.op = SLJIT_IMM;
        sv->val.opw = -1;
        break;
      case 0x10: // table.size
      {
        uint32_t tabidx = read_LEB_signed(m->bytes, (uint32_t *)&m->pc, 32);
        opgen_GenTableSize(m, tabidx);
      }
      break;
      case 0x11: // table.fill
      {
        uint32_t tabidx = read_LEB_signed(m->bytes, (uint32_t *)&m->pc, 32);
        opgen_GenTableFill(m, tabidx);
      }
      break;
      default:
        wa_debug("unrecognized misc opcode 0xfc 0x%x at %d", opcode, m->pc);
        return "unrecognized opcode";
      }
      return NULL;
    }

    char *opgen_GenMiscOp(ModuleCompiler *m, int opcode)
    {
      int32_t tidx, fidx, opcode2;
      uint8_t *bytes = m->bytes;
      switch (opcode)
      {
      case 0xd0:                                               // ref.null
        tidx = read_LEB_signed(bytes, (uint32_t *)&m->pc, 32); // ref type
        opgen_GenRefNull(m, tidx);
        break;
      case 0xd1: // ref.isnull
        opgen_GenIsNull(m);
        break;
      case 0xd2: // ref.func
        fidx = read_LEB(bytes, (uint32_t *)&m->pc, 32);
        opgen_GenRefFunc(m, fidx);
        break;
      case 0xfc: // misc prefix
        opcode2 = m->bytes[m->pc];
        m->pc++;
        ReturnIfErr(opgen_GenMiscOp_FC(m, opcode2));
        break;
      default:
        wa_debug("unrecognized opcode 0x%x at %d", opcode, m->pc);
        return "unrecognized opcode";
      }
      return NULL;
    }

    // if fn is stub/inline function, generate native code and return 1, else return 0.
    int ewa_CheckAndGenStubFunction(ModuleCompiler *m, WasmFunctionEntry fn)
    {
      int i1;
      ewa_get_builtin_symbols(&i1);
      if (fn == InlineFuncList.get_self_runtime_context)
      {
        opgen_GenRefConst(m, m->context);
        return 1;
      }
      else if (fn == InlineFuncList.ref_from_i64)
      {
        if (m->target_ptr_size == 32)
        {
          opgen_GenConvertOp(m, WASMOPC_i32_wrap_i64);
        }
        m->stack[m->sp].wasm_type = WVT_REF;
        return 1;
      }
      else if (fn == InlineFuncList.i64_from_ref)
      {
        if (m->target_ptr_size == 32)
        {
          m->stack[m->sp].wasm_type = WVT_I32;
          opgen_GenConvertOp(m, WASMOPC_i64_extend_i32_u);
        }
        m->stack[m->sp].wasm_type = WVT_I64;
        return 1;
      }
      else if (fn == InlineFuncList.ref_from_index)
      {
        StackValue *sv = m->stack + m->sp - 1;
        SLJIT_ASSERT((sv->jit_type == SVT_GENERAL) && (sv->val.op == SLJIT_IMM));
        opgen_GenBaseAddressReg(m, sv->val.opw);
        stackvalue_EmitSwapTopTwoValue(m);
        m->sp--;
        return 1;
      }
      return 0;
    }

  }

}