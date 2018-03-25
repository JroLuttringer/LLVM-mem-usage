#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/LLVMContext.h"


#include <sstream>

using namespace llvm;
namespace {
  struct ProjetPass : public ModulePass {
    static char ID;
    ProjetPass() : ModulePass(ID) {}



    virtual bool runOnModule(Module &M) 
    { 
        Constant* load = ConstantDataArray::getString(M.getContext(), "I am loading address %p\n", ".ldStr", true);
        Constant* store = ConstantDataArray::getString(M.getContext(),"I am storing %ld at address %p\n", ".stStr", true);
        
        GlobalVariable* gvar_load = new GlobalVariable(M, ArrayTy_0,
            true, GlobalValue::PrivateLinkage, 0, ".str");
        GlobalVariable* gvar_store = new GlobalVariable(M, ArrayTy_0,
            true, GlobalValue::PrivateLinkage, 0, ".str");
        
        gvar_load.setInitializer(load);
        gvar_load.setInitializer(store);
      
    }
    
  };
}

// Definition and initialization of the static ID field
// of class
char ProjetPass::ID = 0;


/////////////////////////////////////////
/// Register the pass for clang usage ///
/////////////////////////////////////////
// http://adriansampson.net/blog/clangpass.html
static void registerProjetPass(const PassManagerBuilder &,
				 legacy::PassManagerBase &PM) {
  PM.add(new ProjetPass());
}
static RegisterStandardPasses
RegisterMyPass(PassManagerBuilder::EP_EnabledOnOptLevel0,
	       registerProjetPass);

