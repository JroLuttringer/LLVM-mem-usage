#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include <llvm/IR/TypeBuilder.h>


#include <sstream>



using namespace llvm;
namespace {
  struct ProjetPass : public ModulePass {
    static char ID;
    ProjetPass() : ModulePass(ID) {}



    virtual bool runOnModule(Module &M) 
    { 
    
        FunctionType* printf_type = TypeBuilder<int(...), false>::get(M.getContext());
        Constant* c = M.getOrInsertFunction("printf", printf_type);
        Function* printfFunction = dyn_cast<Function>(c);
        
        // strings
        const char* LOAD_STR  = "I am loading address %p\n";
        const char* STORE_STR = "I am storing %ld at address %p\n";
        // Create contants
        Constant* load  = ConstantDataArray::getString(M.getContext(), LOAD_STR, ".ldStr");
        Constant* store = ConstantDataArray::getString(M.getContext(), STORE_STR, ".stStr");
        
        //Create ArrayTypes
        ArrayType *Ty_load  = ArrayType::get(Type::getInt8Ty(M.getContext()), strlen(LOAD_STR)+1);
        ArrayType *Ty_store = ArrayType::get(Type::getInt8Ty(M.getContext()), strlen(STORE_STR)+1);
        
        // Create GlobalVar
        GlobalVariable* gvar_load  = new GlobalVariable(M, Ty_load, true, GlobalValue::ExternalLinkage, 0, ".str");
        GlobalVariable* gvar_store = new GlobalVariable(M, Ty_store, true, GlobalValue::ExternalLinkage, 0, ".str");
        
       
        //set initializers
        gvar_load->setInitializer(load);
        gvar_store->setInitializer(store);
        
        
        for (Function & F : M){
            for (BasicBlock &B : F ) {
            	for (Instruction &I : B){
            	    if(LoadInst* LI = dyn_cast<LoadInst>(&I)){
            	        IRBuilder<> builder(&I);
            	        ConstantInt* zero = ConstantInt::get(M.getContext(), APInt(32, 0, false));
            	        Value* zero_value = dyn_cast<Value>(zero);
            	        std::vector<Value*> values;
            	        values.push_back(zero);
            	        values.push_back(zero);
            	        Value* gep = builder.CreateGEP(load,values,"");
            	        builder.CreateCall(printfFunction, gep);
                	    
            	     
            	    } 
            	    
            	    if(StoreInst* SI = dyn_cast<StoreInst>(&I)){
            	        /*ArrayRef<Value*> args = {/*store, SI->getValueOperand(), SI->getPointerOperand()  };
                        IRBuilder<> Builder(&I);
                        Builder.CreateCall(printfFunction, args);  */      	      
            	    }
            	    
            	    if(AllocaInst* AI = dyn_cast<AllocaInst>(&I)){
            	        outs() << "alloca" << "\n";
            	    }
                  
                }
            }
        }
      
    }
    
  };
}

// Definition and initialization of the static ID field
// of class
char ProjetPass::ID = 0;


static RegisterPass<ProjetPass> ProjetPass("projetpass", "pass");

/////////////////////////////////////////
/// Register the pass for clang usage ///
/////////////////////////////////////////
// http://adriansampson.net/blog/clangpass.html 
/*
static void registerProjetPass(const PassManagerBuilder &,
				 legacy::PassManagerBase &PM) {
  PM.add(new ProjetPass());
}
static RegisterStandardPasses
RegisterMyPass(PassManagerBuilder::EP_EnabledOnOptLevel0,
	       registerProjetPass);*/

