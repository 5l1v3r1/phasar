/*
 * MonotoneSolverTest.hh
 *
 *  Created on: 06.06.2017
 *      Author: philipp
 */

#ifndef INTRAMONOTONESOLVERTEST_HH_
#define INTRAMONOTONESOLVERTEST_HH_

#include "../../icfg/LLVMBasedCFG.hh"
#include "../../monotone/IntraMonotoneProblem.hh"
#include <algorithm>
#include <iostream>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
using namespace std;

class IntraMonotoneSolverTest
    : public IntraMonotoneProblem<const llvm::Instruction *,
                                  const llvm::Value *, const llvm::Function *,
                                  LLVMBasedCFG &> {
public:
  IntraMonotoneSolverTest(LLVMBasedCFG &Cfg, const llvm::Function *F);
  virtual ~IntraMonotoneSolverTest() = default;
  
  virtual MonoSet<const llvm::Value *>
  join(const MonoSet<const llvm::Value *> &Lhs,
       const MonoSet<const llvm::Value *> &Rhs) override;
  
  virtual bool sqSubSetEqual(const MonoSet<const llvm::Value *> &Lhs,
                             const MonoSet<const llvm::Value *> &Rhs) override;
  
  virtual MonoSet<const llvm::Value *>
  flow(const llvm::Instruction *S,
       const MonoSet<const llvm::Value *> &In) override;
  
  virtual MonoMap<const llvm::Instruction *, MonoSet<const llvm::Value *>>
  initialSeeds() override;
};

#endif
