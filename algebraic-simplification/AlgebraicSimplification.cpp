#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
  struct AlgebraicSimplification : public FunctionPass {
    static char ID;
    std::vector<Instruction *> InstructionsForRemoval;

    AlgebraicSimplification() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      for (BasicBlock &BB : F) {
        for (Instruction &Instr : BB) {

          if (BinaryOperator *BinaryOp = dyn_cast<BinaryOperator>(&Instr)) {

            Value *LeftOp = BinaryOp->getOperand(0);
            Value *RightOp = BinaryOp->getOperand(1);

            IRBuilder<> Builder(&Instr);

          //Addiction
            if (BinaryOp->getOpcode() == Instruction::Add) {

              // x + 0
              if (ConstantInt *ConstRight = dyn_cast<ConstantInt>(RightOp)){
                if (ConstRight->isZero()) {
                  Instr.replaceAllUsesWith(LeftOp);
                  InstructionsForRemoval.push_back(&Instr);
                }

              // 0 + x
              } else if (ConstantInt *ConstLeft = dyn_cast<ConstantInt>(LeftOp)){
                if (ConstLeft->isZero()) {
                  Instr.replaceAllUsesWith(RightOp);
                  InstructionsForRemoval.push_back(&Instr);
                }

              // x + (-x)
              } else if (BinaryOperator *RightBO = dyn_cast<BinaryOperator>(RightOp)) {
                bool zero = false;
                if(ConstantInt *ConstInt = dyn_cast<ConstantInt>(RightBO->getOperand(0)))
                  zero = ConstInt->isZero();
                if (RightBO->getOpcode() == Instruction::Sub &&
                    zero &&
                    RightBO->getOperand(1) == LeftOp) {

                  Instr.replaceAllUsesWith(ConstantInt::get(BinaryOp->getType(), 0));
                  InstructionsForRemoval.push_back(&Instr);
                }
              }

          //Subtraction
            } else if (BinaryOp->getOpcode() == Instruction::Sub) {

              // x - 0
              if (ConstantInt *ConstRight = dyn_cast<ConstantInt>(RightOp)){
                if (ConstRight->isZero()) {
                  Instr.replaceAllUsesWith(LeftOp);
                  InstructionsForRemoval.push_back(&Instr);
                }

              // 0 - x
              } else if (ConstantInt *ConstLeft = dyn_cast<ConstantInt>(LeftOp)){
                if (ConstLeft->isZero()) {
                  Value *Neg = Builder.CreateNeg(RightOp);
                  Instr.replaceAllUsesWith(Neg);
                  InstructionsForRemoval.push_back(&Instr);
                }

              // x - x
              } else if (LeftOp == RightOp) {
                Instr.replaceAllUsesWith(ConstantInt::get(BinaryOp->getType(), 0));
                InstructionsForRemoval.push_back(&Instr);
              }

          //Multiplication
            } else if (BinaryOp->getOpcode() == Instruction::Mul) {

              // x * 0
              if (ConstantInt *ConstRight = dyn_cast<ConstantInt>(RightOp)){
                if (ConstRight->isZero()) {
                  Instr.replaceAllUsesWith(ConstantInt::get(BinaryOp->getType(), 0));
                  InstructionsForRemoval.push_back(&Instr);
                }

              // 0 * x
              } else if (ConstantInt *ConstLeft = dyn_cast<ConstantInt>(LeftOp)){
                if (ConstLeft->isZero()) {
                  Instr.replaceAllUsesWith(ConstantInt::get(BinaryOp->getType(), 0));
                  InstructionsForRemoval.push_back(&Instr);
                }

              // x * 1
              } else if (ConstantInt *ConstRight = dyn_cast<ConstantInt>(RightOp)){
                if (ConstRight->isOne()) {
                  Instr.replaceAllUsesWith(LeftOp);
                  InstructionsForRemoval.push_back(&Instr);
                }

              // 1 * x
              } else if (ConstantInt *ConstLeft = dyn_cast<ConstantInt>(LeftOp)){
                if (ConstLeft->isOne()) {
                  Instr.replaceAllUsesWith(RightOp);
                  InstructionsForRemoval.push_back(&Instr);
                }

              // x * (-1)
              } else if (ConstantInt *ConstRight = dyn_cast<ConstantInt>(RightOp)){
                if (ConstRight->isMinusOne()) {
                  Value *Neg = Builder.CreateNeg(LeftOp);
                  Instr.replaceAllUsesWith(Neg);
                  InstructionsForRemoval.push_back(&Instr);
                }

              // (-1) * x
              } else if (ConstantInt *ConstLeft = dyn_cast<ConstantInt>(LeftOp)){
                if (ConstLeft->isMinusOne()) {
                  Value *Neg = Builder.CreateNeg(RightOp);
                  Instr.replaceAllUsesWith(Neg);
                  InstructionsForRemoval.push_back(&Instr);
                }
              }

          //Division
            } else if (BinaryOp->getOpcode() == Instruction::SDiv) {

              // x / 1
              if (ConstantInt *ConstRight = dyn_cast<ConstantInt>(RightOp)){
                if (ConstRight->isOne()) {
                  Instr.replaceAllUsesWith(LeftOp);
                  InstructionsForRemoval.push_back(&Instr);
                }

              // 0 / x
              } else if (ConstantInt *ConstLeft = dyn_cast<ConstantInt>(LeftOp)){
                if (ConstLeft->isZero()) {
                  if(ConstantInt *ConstRight = dyn_cast<ConstantInt>(RightOp)){
                    if(!ConstRight->isZero()){
                      Instr.replaceAllUsesWith(ConstantInt::get(BinaryOp->getType(), 0));
                      InstructionsForRemoval.push_back(&Instr);
                    }
                  }
                }

              // x / x
              } else if (LeftOp == RightOp) {
                if(ConstantInt *ConstLeft = dyn_cast<ConstantInt>(LeftOp)){
                  if(!ConstLeft->isZero()){
                    Instr.replaceAllUsesWith(ConstantInt::get(BinaryOp->getType(), 1));
                    InstructionsForRemoval.push_back(&Instr);
                  }
                }
              }
            }
          }

        }
      }

      //Instructios removal
      for (Instruction *I : InstructionsForRemoval)
        I->eraseFromParent();

      InstructionsForRemoval.clear();

      return true;
    }


  };
}

char AlgebraicSimplification::ID = 0;
static RegisterPass<AlgebraicSimplification> X("algebraic-pass", "This pass simplifies algebraic expressions.", false, false);