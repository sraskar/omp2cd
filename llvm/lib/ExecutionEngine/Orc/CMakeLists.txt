add_llvm_library(LLVMOrcJIT
  ExecutionUtils.cpp
  IndirectionUtils.cpp
  NullResolver.cpp
  OrcABISupport.cpp
  OrcCBindings.cpp
  OrcCBindingsStack.cpp
  OrcError.cpp
  OrcMCJITReplacement.cpp
  OrcRemoteTargetRPCAPI.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/ExecutionEngine/Orc

  DEPENDS
  intrinsics_gen
  )
