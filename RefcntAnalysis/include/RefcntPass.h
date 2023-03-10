//
// Created by ziqi on 06/03/2023
//

#ifndef REFCNTPASS_H
#define REFCNTPASS_H

#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Use.h>
#include <llvm/IR/Value.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <set>
#include <vector>

#define INCREF_STR "_Py_INCREF"
#define XINCREF_STR "_Py_XINCREF"
#define DECREF_STR "_Py_DECREF"
#define XDECREF_STR "_Py_XDECREF"

using namespace llvm;

namespace {
    struct RefcntPass : public ModulePass {

        /**
         * @brief describe API that returns special obj
         */
        struct srcAPI{
            std::string fun_name;
            /// more info goes here
            // consider return type ???


            ///
        };
        /**
         * @brief describe API that has special behaviors to the i'th argument
         */
        struct sinkAPI {
            std::string fun_name;
            std::vector<int> sinks; // the i'th argument will be influenced but may be more than one
            /// more info goes here


            ///
        };

    private:
        /// basic properties
        std::string AnalysesMode = "inter";
        std::string EntryFunction = "main";

        ///special APIs
        std::set<srcAPI> retStrongAPI;
        std::set<srcAPI> retWeakAPI;
        std::set<sinkAPI> argBorrowAPI;
        std::set<sinkAPI> argStealAPI;
        /**
         * @brief initialize data structures and prepare arguments
         */
        void init_pass();

        /**
         * @brief intraprocedual analysis
         * @param cur_func analysis function
         */
        void intraAnalysis(Function *cur_func);

        /**
         * @brief inter analysis
         * @param cur_func
         */
        void interAnalysis(Function *cur_func);

        /**
         * @brief do more bug report before leaving a function,
         * feel free to modify and add parameters if needed.
         */
        void CheckBeforeRet();

    public:
        /**
         * @brief analysis ID
         */
        static char ID;

        RefcntPass() : ModulePass(ID) {}

        /**
         * @brief llvm will enter the pass through this function, do initialize and
         * @param M
         * @return
         */
        bool runOnModule(Module &M);

        void refcntAnalysis(Function *fun_entry);
    };
}

#endif // REFCNTPASS_H