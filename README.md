Le dossier Projet-CA doit etre dans le dossier où est installé clang et opt 5.0
Dans l'exemple des TP le dossier Projet-CA serait fils de tp-llvm

Dans le dossier build : 
	cmake ..
	make

Puis dans le dossier Projet-CA: 
	clang -o test.bin -Xclang -load -Xclang $PWD/build/libLLVMProjetPass.so test.c