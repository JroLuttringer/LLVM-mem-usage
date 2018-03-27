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
 	// Create printf function   
        FunctionType* printf_type = TypeBuilder<int(char*,...), false>::get(M.getContext());
        Constant* c = M.getOrInsertFunction("printf", printf_type);
        Function* printfFunction = dyn_cast<Function>(c);
        
        // messages
        const char* LOAD_STR  = "I am loading address %p\n";
        const char* STORE_STR = "I am storing %ld at address %p\n";
        // const char* ALLOC_STR = "I am allocating %ld elements\n";
        
        // Create contants
        Constant* load  = ConstantDataArray::getString(M.getContext(), LOAD_STR, ".ldStr");
        Constant* store = ConstantDataArray::getString(M.getContext(), STORE_STR, ".stStr");
        // Constant* alloc = ConstantDataArray::getString(M.getContext(), ALLOC_STR, ".alStr");
        
        //Create ArrayTypes
        ArrayType *Ty_load  = ArrayType::get(Type::getInt8Ty(M.getContext()), strlen(LOAD_STR)+1);
        ArrayType *Ty_store = ArrayType::get(Type::getInt8Ty(M.getContext()), strlen(STORE_STR)+1);
        // ArrayType *Ty_alloc = ArrayType::get(Type::getInt8Ty(M.getContext()), strlen(ALLOC_STR)+1);
        
        // Create GlobalVar
        GlobalVariable* gvar_load  = new GlobalVariable(M, Ty_load, true, GlobalValue::ExternalLinkage, 0, ".ldStr");
        GlobalVariable* gvar_store = new GlobalVariable(M, Ty_store, true, GlobalValue::ExternalLinkage, 0, ".stStr");
        // GlobalVariable* gvar_alloc = new GlobalVariable(M, Ty_alloc, true, GlobalValue::ExternalLinkage, 0, ".alStr");
        
       
        //set initializers
        gvar_load->setInitializer(load);
        gvar_store->setInitializer(store);
        // gvar_alloc->setInitializer(alloc);
        
        for (Function & F : M){
            for (BasicBlock &B : F ) {
            	for (Instruction &I : B){
            	    if(LoadInst* LI = dyn_cast<LoadInst>(&I)){
                        // Create builder
            	        IRBuilder<> builder(&I);
            	        ConstantInt* zero = ConstantInt::get(M.getContext(), APInt(32, 0, false));
            	        
                        // Create values for GEP
                        Value* zero_value = dyn_cast<Value>(zero);
            	        std::vector<Value*> values;
            	        values.push_back(zero);
            	        values.push_back(zero);
            	        Value* gep = builder.CreateGEP(load->getType(),gvar_load,values,"");
            	        
                        // printf args
            	        ArrayRef<Value*> args = {gep, LI->getPointerOperand()};
                        // Add printf call to IR
            	        builder.CreateCall(printfFunction, args);
            	     
            	    } 
            	    
            	    if(StoreInst* SI = dyn_cast<StoreInst>(&I)){
            	        // builder & values for GEP
                        IRBuilder<> builder(&I);
            	        ConstantInt* zero = ConstantInt::get(M.getContext(), APInt(32, 0, false));
            	        Value* zero_value = dyn_cast<Value>(zero);
            	        std::vector<Value*> values;
            	        values.push_back(zero);
            	        values.push_back(zero);
            	        Value* gep = builder.CreateGEP(store->getType(),gvar_store,values,"");
            	        
                        // print call with arguments
            	        ArrayRef<Value*> args = {gep, SI->getValueOperand(), SI->getPointerOperand()};
            	        builder.CreateCall(printfFunction, args);     	      
            	    }

                    /*
            	    
            	    if(AllocaInst* AI = dyn_cast<AllocaInst>(&I)){
            	       // Create builder
            	        IRBuilder<> builder(&I);
            	        ConstantInt* zero = ConstantInt::get(M.getContext(), APInt(32, 0, false));
            	        
                        // Create values for GEP
                        Value* zero_value = dyn_cast<Value>(zero);
            	        std::vector<Value*> values;
            	        values.push_back(zero);
            	        values.push_back(zero);
            	        Value* gep = builder.CreateGEP(alloc->getType(),gvar_alloc,values,"");
            	        
                        // printf args
            	        ArrayRef<Value*> args = {gep, AI->getArraySize()};
                        // Add printf call to IR
            	        builder.CreateCall(printfFunction, args);
                          
                    }

                    */
                  
                }
            }
        }
      
    }
    
  };
}

// Definition and initialization of the static ID field
// of class
char ProjetPass::ID = 0;


// static RegisterPass<ProjetPass> ProjetPass("projetpass", "pass");

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

