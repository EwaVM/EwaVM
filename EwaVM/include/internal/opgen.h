#pragma once

#include "def.h"

namespace EwaVM
{

   namespace OpGen
   {

      // opgen.cpp
      void SkipImmediates(uint8_t *bytes, uint32_t *pos);
      char *PrepareFunc(ModuleCompiler *m);
      char *GenCode(ModuleCompiler *m);
      char *EmitFunction(ModuleCompiler *m, WasmFunction *func);
      void FreeFunction(WasmFunctionEntry code);

      // extfunc.c
      extern Type func_type_ret_void;
      extern Type func_type_ret_i32;
      extern Type func_type_ret_i64;
      extern Type func_type_ret_f32;
      extern Type func_type_ret_f64;
      extern Type func_type_i64_ret_i32;
      extern Type func_type_i64_ret_i64;
      extern Type func_type_i32_ret_i32;
      extern Type func_type_i64_i64_ret_i32;
      extern Type func_type_f32_ret_f32;
      extern Type func_type_f64_ret_f64;
      extern Type func_type_i32_i32_ret_i32;
      extern Type func_type_i64_i64_ret_i64;
      extern Type func_type_f32_f32_ret_f32;
      extern Type func_type_f64_f64_ret_f64;
      extern Type func_type_f32_ret_i32;
      extern Type func_type_f64_ret_i32;
      extern Type func_type_f32_ret_i64;
      extern Type func_type_f64_ret_i64;
      extern Type func_type_i32_ret_f32;
      extern Type func_type_i64_ret_f32;
      extern Type func_type_i32_ret_f64;
      extern Type func_type_i64_ret_f64;
      extern Type func_type_i32_i32_ref_ret_i32;
      extern Type func_type_ref_ref_i32_void;
      extern Type func_type_memoryfill;

      void insn_i64eqz(void *fp);
      void insn_i64clz(void *fp);
      void insn_i64ctz(void *fp);
      void insn_i64popcount(void *fp);
      void insn_i32clz(void *fp);
      void insn_i32ctz(void *fp);
      void insn_i32popcount(void *fp);
      void insn_i64eq(void *fp);
      void insn_i64ne(void *fp);
      void insn_i64lts(void *fp);
      void insn_i64ltu(void *fp);
      void insn_i64gts(void *fp);
      void insn_i64gtu(void *fp);
      void insn_i64les(void *fp);
      void insn_i64leu(void *fp);
      void insn_i64ges(void *fp);
      void insn_i64geu(void *fp);
      void insn_f32ceil(float *fp);
      void insn_f32floor(float *fp);
      void insn_f32trunc(float *fp);
      void insn_f32nearest(float *fp);
      void insn_f32sqrt(float *fp);
      void insn_f64ceil(double *fp);
      void insn_f64floor(double *fp);
      void insn_f64trunc(double *fp);
      void insn_f64nearest(double *fp);
      void insn_f64sqrt(double *fp);
      void insn_i32rotl(uint32_t *fp);
      void insn_i32rotr(uint32_t *fp);
      void insn_i64rotl(uint64_t *fp);
      void insn_i64rotr(uint64_t *fp);
      void insn_i64mul(uint64_t *fp);
      void insn_i64divs(int64_t *fp);
      void insn_i64divu(uint64_t *fp);
      void insn_i64rems(int64_t *fp);
      void insn_i64remu(uint64_t *fp);
      void insn_i64shl(uint64_t *fp);
      void insn_i64shrs(int64_t *fp);
      void insn_i64shru(uint64_t *fp);
      void insn_f32max(float *fp);
      void insn_f32min(float *fp);
      void insn_f32copysign(float *fp);
      void insn_f64max(double *fp);
      void insn_f64min(double *fp);
      void insn_f64copysign(double *fp);
      void insn_i32truncf32u(uint32_t *fp);
      void insn_i32truncf64u(uint32_t *fp);
      void insn_i64truncf32s(int64_t *fp);
      void insn_i64truncf32u(uint64_t *fp);
      void insn_i64truncf64s(int64_t *fp);
      void insn_i64truncf64u(uint64_t *fp);
      void insn_f32converti32u(float *fp);
      void insn_f32converti64s(float *fp);
      void insn_f32converti64u(float *fp);
      void insn_f64converti32u(double *fp);
      void insn_f64converti64s(double *fp);
      void insn_f64converti64u(double *fp);

      
      void insn_memorygrow(void *fp);
      void insn_version(uint32_t *fp);
      void insn_memory_alloc(void *fp);
      void insn_memory_free(void *fp);
      void insn_memoryfill(void *fp);
      void insn_native_index_size(void *fp);
      void insn_ref_from_i64(void *fp);
      void insn_i64_from_ref(void *fp);
      void insn_get_self_runtime_context(void *fp);
      void insn_ref_from_index(void *fp);
      void insn_ref_copy_bytes(void *fp);
      void insn_ref_string_length(void *fp);
      void insn_load_module(void *fp);
      void insn_unload_module(void *fp);
      void insn_import(void *fp);
      void waexpr_run_const(ModuleCompiler *m, void *result);
      void debug_printtypes(char *t);
      void debug_printfunctype(Type *type);
      void insn_host_definition(void *fp);
      void insn_fopen(void *fp);
      void insn_fread(void *fp);
      void insn_fwrite(void *fp);
      void insn_fclose(void *fp);
      void insn_stdio(void *fp);

      // opgen_ctl.c
      void opgen_GenUnreachable(ModuleCompiler *m);
      void opgen_GenBlock(ModuleCompiler *m);
      void opgen_GenLoop(ModuleCompiler *m);
      void opgen_GenIf(ModuleCompiler *m);
      void opgen_GenElse(ModuleCompiler *m);
      void opgen_GenEnd(ModuleCompiler *m);
      void opgen_GenBr(ModuleCompiler *m, int32_t depth);
      void opgen_GenBrIf(ModuleCompiler *m, int32_t depth);
      void opgen_GenBrTable(ModuleCompiler *m);
      void opgen_GenReturn(ModuleCompiler *m);
      void opgen_GenCall(ModuleCompiler *m, int32_t fidx);
      void opgen_GenCallIndirect(ModuleCompiler *m, int32_t typeidx,
                                 int32_t tableidx);
      void opgen_GenDrop(ModuleCompiler *m);
      void opgen_GenSelect(ModuleCompiler *m);
      char *opgen_GenCtlOp(ModuleCompiler *m, int opcode);

      void opgen_GenLocalGet(ModuleCompiler *m, uint32_t idx);
      void opgen_GenLocalSet(ModuleCompiler *m, uint32_t idx);
      void opgen_GenLocalTee(ModuleCompiler *m, uint32_t idx);
      void opgen_GenGlobalGet(ModuleCompiler *m, uint32_t idx);
      void opgen_GenGlobalSet(ModuleCompiler *m, uint32_t idx);
      void opgen_GenBaseAddressRegForTable(ModuleCompiler *m, uint32_t tabidx);
      void opgen_GenTableGet(ModuleCompiler *m, uint32_t idx);
      void opgen_GenTableSet(ModuleCompiler *m, uint32_t idx);
      void opgen_GenCurrentMemory(ModuleCompiler *m, uint32_t index);
      void opgen_GenMemoryGrow(ModuleCompiler *m, uint32_t index);
      void opgen_GenI32Load(ModuleCompiler *m, int32_t opcode, int offset);
      void opgen_GenI64Load(ModuleCompiler *m, int32_t opcode, sljit_sw offset);
      void opgen_GenFloatLoad(ModuleCompiler *m, int32_t opcode, uint32_t offset);
      void opgen_GenBaseAddressReg(ModuleCompiler *m, uint32_t midx);
      void opgen_GenLoad(ModuleCompiler *m, int32_t opcode, sljit_sw offset,
                         sljit_sw align, sljit_uw memindex);
      void opgen_GenStore(ModuleCompiler *m, int32_t opcode, sljit_sw offset,
                          sljit_sw align, sljit_uw memindex);
      void opgen_GenMemoryCopy(ModuleCompiler *m, int dmidx, int smidx);
      void opgen_GenTableCopy(ModuleCompiler *m, int32_t dtab, int32_t stab);
      void opgen_GenMemoryFill(ModuleCompiler *m, int midx);
      void opgen_GenTableFill(ModuleCompiler *m, int32_t tabidx);
      char *opgen_GenMemOp(ModuleCompiler *m, int opcode);

      // opgen_misc.c
      void opgen_GenRefNull(ModuleCompiler *m, int32_t typeidx);
      void opgen_GenIsNull(ModuleCompiler *m);
      void opgen_GenRefFunc(ModuleCompiler *m, int32_t fidx);
      void opgen_GenTableSize(ModuleCompiler *m, int tabidx);
      char *opgen_GenMiscOp_FC(ModuleCompiler *m, int opcode);
      char *opgen_GenMiscOp(ModuleCompiler *m, int opcode);
      int ewa_CheckAndGenStubFunction(ModuleCompiler *m, WasmFunctionEntry fn);

      // opgen_num.c
      void opgen_GenCompareOp(ModuleCompiler *m, int opcode);
      void opgen_GenArithmeticOp(ModuleCompiler *m, int opcode);
      void opgen_GenConvertOp(ModuleCompiler *m, int opcode);
      char *opgen_GenNumOp(ModuleCompiler *m, int opcode);

      // opgen_utils.c
      uint32_t wasmtype_GetSizeAndAlign(int wasm_type, uint32_t *align);
      int stackvalue_IsFloat(StackValue *sv);
      uint32_t stackvalue_GetSizeAndAlign(StackValue *sv, uint32_t *align);
      uint32_t stackvalue_GetAlignedOffset(StackValue *sv, uint32_t offset, uint32_t *nextOffset);
      void stackvalue_HighWord(ModuleCompiler *m, StackValue *sv, sljit_s32 *op,
                               sljit_sw *opw);
      void stackvalue_LowWord(ModuleCompiler *m, StackValue *sv, sljit_s32 *op,
                              sljit_sw *opw);
      StackValue *stackvalue_FindSvalueUseReg(ModuleCompiler *m, sljit_s32 r,
                                              sljit_s32 regtype, int upstack);
      int ewa_EmitStoreStackValue(ModuleCompiler *m, StackValue *sv, int memreg,
                                  int offset);
      // 32bit arch only
      int ewa_EmitSaveStack(ModuleCompiler *m, StackValue *sv);
      int ewa_EmitSaveStackAll(ModuleCompiler *m);
      int ewa_EmitLoadReg(ModuleCompiler *m, StackValue *sv, int reg);
      inline size_t get_funcarr_offset(ModuleCompiler *m);
      StackValue *stackvalue_Push(ModuleCompiler *m, int32_t wasm_type);
      StackValue *stackvalue_PushStackValueLike(ModuleCompiler *m, StackValue *refsv);
      void stackvalue_EmitSwapTopTwoValue(ModuleCompiler *m);
      int ewa_EmitCallFunc(ModuleCompiler *m, Type *type, sljit_s32 memreg,
                           sljit_sw offset);
      int ewa_EmitFuncReturn(ModuleCompiler *m);
      int stackvalue_AnyRegUsedBySvalue(StackValue *sv);
      sljit_s32 ewa_GetFreeReg(ModuleCompiler *m, sljit_s32 regtype, int upstack);
      sljit_s32 ewa_GetFreeRegExcept(ModuleCompiler *m, sljit_s32 regtype,
                                     sljit_s32 except, int upstack);
      void ewa_EmitStackValueLoadReg(ModuleCompiler *m, StackValue *sv);
      int ewa_EmitFuncEnter(ModuleCompiler *m);
      void opgen_GenI32Const(ModuleCompiler *m, uint32_t c);
      void opgen_GenI64Const(ModuleCompiler *m, uint64_t c);
      void opgen_GenF32Const(ModuleCompiler *m, uint8_t *c);
      void opgen_GenF64Const(ModuleCompiler *m, uint8_t *c);
      void opgen_GenRefConst(ModuleCompiler *m, void *c);
   };

};

using namespace EwaVM::OpGen;