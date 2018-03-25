#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/LLVMContext.h"


#include <sstream>

using namespace llvm;
int i = 0;

void printStr(char *toPrint) {
  if (toPrint) {
    fprintf(stderr, "%s", toPrint);
  }
  else {
    fprintf(stderr, "::printStr(...):NULL arg.\n");
  }
}

void generateStringPrint(llvm::LLVMContext &context,
                         llvm::Module &module,
                         llvm::IRBuilder<> &builder,
                         std::string toPrint,
                         bool useGlobal ) {
  printf("Error is here %d\n", i++);
  llvm::Function *printFunct = module.getFunction("printStr");
  printf("Error is here %d\n", i++);
  llvm::GlobalVariable *stringVar;
  llvm::Constant *stringConstant =
  llvm::ConstantDataArray::getString(context, toPrint);
  printf("Error is here %d\n", i++);

  //Ici il y a un probleme avec l'allocation de l'espace pour la variable à imprimer
  //A mon avis d'ou le fait qu'il faille instancier des variables globales
  //Du coup on aura à les load et pas les allouer
  stringVar =
    new llvm::GlobalVariable(module,
                             stringConstant->getType(),
                             true,
                             llvm::GlobalValue::PrivateLinkage,
                             stringConstant,
                             "");
    stringVar->print(errs());
    printf("\n");
    module.getGlobalList().push_back(stringVar);
    // stringVar = builder.CreateAlloca(stringConstant->getType());
  // printf("Error is here %d\n", i++);
  //   builder.CreateStore(stringConstant, stringVar);
  printf("Error is here %d\n", i++);
  

  llvm::Value *cast = builder.CreatePointerCast(stringVar,
                                                builder.getInt8PtrTy());
  printf("Error is here %d\n", i++);
  // builder.CreateCall(printFunct, cast);
  printf("Error is here %d\n", i++);
}

// The Pass itself
namespace {
  struct ProjetPass : public ModulePass {
    static char ID;
    ProjetPass() : ModulePass(ID) {}



    virtual bool runOnModule(Module &M) 
    { 
      //Ici il faut ajouter les strings load et store aux variables globales

      // for (auto& Global : M.getGlobalList()){
          
      //  }

      //Un example de fonction pour print un int et un string se trouve dans 
       // llvm-5.0.0.src/examples/ExceptionDemo
      llvm::LLVMContext Context;
      llvm::IRBuilder<> builder(Context);
      std::ostringstream bufferToPrint;
      bufferToPrint << "Gen: Executing finally block ";

       // generateStringPrint(Context, M, builder, bufferToPrint.str(), false);

      llvm::Constant *stringConstant =
      llvm::ConstantDataArray::getString(Context, bufferToPrint.str());
      printf("Error is here %d\n", i++);

  //Ici il y a un probleme avec l'allocation de l'espace pour la variable à imprimer
  //A mon avis d'ou le fait qu'il faille instancier des variables globales
  //Du coup on aura à les load et pas les allouer
  GlobalVariable* stringVar =
    new llvm::GlobalVariable(M,
                             stringConstant->getType(),
                             true,
                             llvm::GlobalValue::PrivateLinkage,
                             stringConstant,
                             "");

        // for (Function &F : M)
        // {
        //   for (BasicBlock &B : F) 
        //   {
        //     for (Instruction &I : B) 
        //     {
        //       if (isa<LoadInst>(I)) // || isa<StoreInst>(I))
        //       {
        //         IRBuilder<> builder(&I);
        //         Value* ptr = I.getOperand(0);
        //         // ArrayRef<Value*> args = {load, ptr};
        //         // builder.createCall(printfFunction, args);

        //         printf("Load instruction this is ptr\n");
        //         ptr->print(errs());
        //         printf(" \n");

        //         printf("Load instruction this is I\n");
        //         I.print(errs());
        //         printf(" \n");
        //       }
        //       else 
        //         if (isa<StoreInst>(I))
        //       {
        //         IRBuilder<> builder(&I);
        //         //This gets the value to be stored
        //         Value* val = I.getOperand(0);
        //         //This gets the name of the variable, we need its address
        //         Value* ptr = I.getOperand(1);
        //         // ArrayRef<Value*> args = {store, ptr, val};
        //         // builder.createCall(printfFunction, args);
        //         printf("Store instruction this is val\n");
        //         val->print(errs());
        //         printf(" \n");
        //         printf("Store instruction this is ptr\n");
        //         ptr->print(errs());
        //         printf(" \n");
        //         printf("Store instruction this is I\n");
        //         I.print(errs());
        //         printf(" \n");
        //       }

        //     }
        //   }
        // }
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

