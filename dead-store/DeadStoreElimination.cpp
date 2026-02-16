#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>

using namespace llvm;

namespace {
struct DeadStoreEliminationPass : public FunctionPass {
    static char ID;
    DeadStoreEliminationPass() : FunctionPass(ID) {}

    bool isDeadStore(StoreInst *SI) {
        Value *Ptr = SI->getPointerOperand();
        
        BasicBlock::iterator it(SI);
        ++it;
        
        for (; it != SI->getParent()->end(); ++it) {
            Instruction *Next = &*it;
            
            if (StoreInst *NextStore = dyn_cast<StoreInst>(Next)) {
                if (NextStore->getPointerOperand() == Ptr) {
                    return true;
                }
            }
            
            if (LoadInst *NextLoad = dyn_cast<LoadInst>(Next)) {
                if (NextLoad->getPointerOperand() == Ptr) {
                    return false;
                }
            }
        }
        
        return true;
    }

    bool runOnFunction(Function &F) override {
        bool Changed = false;
        std::vector<StoreInst*> DeadStores;
        
        for (BasicBlock &BB : F) {
            for (Instruction &I : BB) {
                if (StoreInst *SI = dyn_cast<StoreInst>(&I)) {
                    if (isDeadStore(SI)) {
                        DeadStores.push_back(SI);
                        errs() << "[DEAD STORE]: " << I << "\n";
                    }
                }
            }
        }
        
        for (StoreInst *SI : DeadStores) {
            SI->eraseFromParent();
            Changed = true;
        }
        
        errs() << "Removed " << DeadStores.size() << " dead stores\n";
        
        return Changed;
    }
};
}

char DeadStoreEliminationPass::ID = 0;
static RegisterPass<DeadStoreEliminationPass> X(
    "dead-store-elimination",
    "Dead Store Elimination Pass",
    false,
    false
);