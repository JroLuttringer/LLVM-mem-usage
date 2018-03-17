#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

// The Pass itself
namespace {
  struct ProjetPass : public FunctionPass {
    static char ID;
    ProjetPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      outs() << "I saw a function called " << F.getName() << "!\n";
			for (auto& U : F.args())
			{
				outs() << "args of f are " << U << " \n";
			}
      return false;
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

