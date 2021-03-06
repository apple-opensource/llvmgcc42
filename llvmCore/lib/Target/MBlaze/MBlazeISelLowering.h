//===-- MBlazeISelLowering.h - MBlaze DAG Lowering Interface ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that MBlaze uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//

#ifndef MBlazeISELLOWERING_H
#define MBlazeISELLOWERING_H

#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/Target/TargetLowering.h"
#include "MBlaze.h"
#include "MBlazeSubtarget.h"

namespace llvm {
  namespace MBlazeCC {
    enum CC {
      FIRST = 0,
      EQ,
      NE,
      GT,
      LT,
      GE,
      LE
    };
  }

  namespace MBlazeISD {
    enum NodeType {
      // Start the numbering from where ISD NodeType finishes.
      FIRST_NUMBER = ISD::BUILTIN_OP_END,

      // Jump and link (call)
      JmpLink,

      // Handle gp_rel (small data/bss sections) relocation.
      GPRel,

      // Select CC Pseudo Instruction
      Select_CC,

      // Wrap up multiple types of instructions
      Wrap,

      // Integer Compare
      ICmp,

      // Return
      Ret
    };
  }

  //===--------------------------------------------------------------------===//
  // TargetLowering Implementation
  //===--------------------------------------------------------------------===//

  class MBlazeTargetLowering : public TargetLowering  {
    int VarArgsFrameIndex;            // FrameIndex for start of varargs area.

  public:

    explicit MBlazeTargetLowering(MBlazeTargetMachine &TM);

    /// LowerOperation - Provide custom lowering hooks for some operations.
    virtual SDValue LowerOperation(SDValue Op, SelectionDAG &DAG);

    /// getTargetNodeName - This method returns the name of a target specific
    //  DAG node.
    virtual const char *getTargetNodeName(unsigned Opcode) const;

    /// getSetCCResultType - get the ISD::SETCC result ValueType
    MVT::SimpleValueType getSetCCResultType(EVT VT) const;

    virtual unsigned getFunctionAlignment(const Function *F) const;
  private:
    // Subtarget Info
    const MBlazeSubtarget *Subtarget;


    // Lower Operand helpers
    SDValue LowerCallResult(SDValue Chain, SDValue InFlag,
                            CallingConv::ID CallConv, bool isVarArg,
                            const SmallVectorImpl<ISD::InputArg> &Ins,
                            DebugLoc dl, SelectionDAG &DAG,
                            SmallVectorImpl<SDValue> &InVals);

    // Lower Operand specifics
    SDValue LowerConstantPool(SDValue Op, SelectionDAG &DAG);
    SDValue LowerGlobalAddress(SDValue Op, SelectionDAG &DAG);
    SDValue LowerGlobalTLSAddress(SDValue Op, SelectionDAG &DAG);
    SDValue LowerJumpTable(SDValue Op, SelectionDAG &DAG);
    SDValue LowerSELECT_CC(SDValue Op, SelectionDAG &DAG);
    SDValue LowerVASTART(SDValue Op, SelectionDAG &DAG);

    virtual SDValue
      LowerFormalArguments(SDValue Chain,
                           CallingConv::ID CallConv, bool isVarArg,
                           const SmallVectorImpl<ISD::InputArg> &Ins,
                           DebugLoc dl, SelectionDAG &DAG,
                           SmallVectorImpl<SDValue> &InVals);

    virtual SDValue
      LowerCall(SDValue Chain, SDValue Callee,
                CallingConv::ID CallConv, bool isVarArg,
                bool &isTailCall,
                const SmallVectorImpl<ISD::OutputArg> &Outs,
                const SmallVectorImpl<ISD::InputArg> &Ins,
                DebugLoc dl, SelectionDAG &DAG,
                SmallVectorImpl<SDValue> &InVals);

    virtual SDValue
      LowerReturn(SDValue Chain,
                  CallingConv::ID CallConv, bool isVarArg,
                  const SmallVectorImpl<ISD::OutputArg> &Outs,
                  DebugLoc dl, SelectionDAG &DAG);

    virtual MachineBasicBlock *EmitInstrWithCustomInserter(MachineInstr *MI,
                                                         MachineBasicBlock *MBB,
                    DenseMap<MachineBasicBlock*, MachineBasicBlock*> *EM) const;

    // Inline asm support
    ConstraintType getConstraintType(const std::string &Constraint) const;

    std::pair<unsigned, const TargetRegisterClass*>
              getRegForInlineAsmConstraint(const std::string &Constraint,
              EVT VT) const;

    std::vector<unsigned>
    getRegClassForInlineAsmConstraint(const std::string &Constraint,
              EVT VT) const;

    virtual bool isOffsetFoldingLegal(const GlobalAddressSDNode *GA) const;

    /// isFPImmLegal - Returns true if the target can instruction select the
    /// specified FP immediate natively. If false, the legalizer will
    /// materialize the FP immediate as a load from a constant pool.
    virtual bool isFPImmLegal(const APFloat &Imm, EVT VT) const;
  };
}

#endif // MBlazeISELLOWERING_H
