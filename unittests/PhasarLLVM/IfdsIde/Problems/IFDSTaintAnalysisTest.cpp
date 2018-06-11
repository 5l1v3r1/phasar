#include <gtest/gtest.h>
#include <phasar/DB/ProjectIRDB.h>
#include <phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h>
#include <phasar/PhasarLLVM/IfdsIde/Problems/IFDSTaintAnalysis.h>
#include <phasar/PhasarLLVM/IfdsIde/Solver/LLVMIFDSSolver.h>
#include <phasar/PhasarLLVM/Pointer/LLVMTypeHierarchy.h>

using namespace psr;

TEST(SecondTest1, SecondTestName1) {
  initializeLogger(true);
  ProjectIRDB IRDB(
      {"../../../../../test/llvm_test_code/control_flow/function_call.ll"},
      IRDBOptions::NONE);
  IRDB.preprocessIR();
  LLVMTypeHierarchy TH(IRDB);
  LLVMBasedICFG ICFG(TH, IRDB, WalkerStrategy::Pointer, ResolveStrategy::OTF,
                     {"main"});
  IFDSTaintAnalysis TaintProblem(ICFG, {"main"});
  LLVMIFDSSolver<const llvm::Value *, LLVMBasedICFG &> TaintSolver(TaintProblem,
                                                                   true);
  TaintSolver.solve();
  std::cout << "Problem has been solved" << std::endl;

  // TaintSolver.ifdsResultsAt()
}

TEST(SecondTest2, SecondTestName2) {
  std::vector<int> iv = {1, 2, 3};
  ASSERT_EQ(3, iv.size());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
