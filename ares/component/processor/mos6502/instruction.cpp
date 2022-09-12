#define op(id, name, ...) case id: return instruction##name(__VA_ARGS__);
#define fp(name) &MOS6502::algorithm##name

auto MOS6502::interrupt() -> void {
  idle();
  idle();
  push(PCH);
  push(PCL);
  n16 vector = 0xfffe;
  nmi(vector);
  push(P | 0x20);
  I = 1;
  PCL = read(vector++);
L PCH = read(vector++);
}

auto MOS6502::instruction() -> void {
  auto code = opcode();

  switch(code) {
  op(0x00, Break)
  op(0x01, IndirectXRead, fp(ORA), A)
  op(0x04, NoOperationZeroPage)
  op(0x05, ZeroPageRead, fp(ORA), A)
  op(0x06, ZeroPageModify, fp(ASL))
  op(0x07, ZeroPageModify, fp(SLO))
  op(0x08, PushP)
  op(0x09, Immediate, fp(ORA), A)
  op(0x0a, Implied, fp(ASL), A)
  op(0x0b, Immediate, fp(ANC), A)
  op(0x0c, NoOperationAbsolute)
  op(0x0d, AbsoluteRead, fp(ORA), A)
  op(0x0e, AbsoluteModify, fp(ASL))
  op(0x0f, AbsoluteModify, fp(SLO))
  op(0x10, Branch, N == 0)
  op(0x11, IndirectYRead, fp(ORA), A)
  op(0x14, NoOperationZeroPage, X)
  op(0x15, ZeroPageRead, fp(ORA), A, X)
  op(0x16, ZeroPageModify, fp(ASL), X)
  op(0x17, ZeroPageModify, fp(SLO), X)
  op(0x18, Clear, C)
  op(0x19, AbsoluteRead, fp(ORA), A, Y)
  op(0x1a, NoOperation)
  op(0x1b, AbsoluteModify, fp(SLO), Y)
  op(0x1c, NoOperationAbsolute, X)
  op(0x1d, AbsoluteRead, fp(ORA), A, X)
  op(0x1e, AbsoluteModify, fp(ASL), X)
  op(0x1f, AbsoluteModify, fp(SLO), X)
  op(0x20, CallAbsolute)
  op(0x21, IndirectXRead, fp(AND), A)
  op(0x24, ZeroPageRead, fp(BIT), A)
  op(0x25, ZeroPageRead, fp(AND), A)
  op(0x26, ZeroPageModify, fp(ROL))
  op(0x28, PullP)
  op(0x29, Immediate, fp(AND), A)
  op(0x2a, Implied, fp(ROL), A)
  op(0x2b, Immediate, fp(ANC), A)
  op(0x2c, AbsoluteRead, fp(BIT), A)
  op(0x2d, AbsoluteRead, fp(AND), A)
  op(0x2e, AbsoluteModify, fp(ROL))
  op(0x30, Branch, N == 1)
  op(0x31, IndirectYRead, fp(AND), A)
  op(0x34, NoOperationZeroPage, X)
  op(0x35, ZeroPageRead, fp(AND), A, X)
  op(0x36, ZeroPageModify, fp(ROL), X)
  op(0x38, Set, C)
  op(0x39, AbsoluteRead, fp(AND), A, Y)
  op(0x3a, NoOperation)
  op(0x3c, NoOperationAbsolute, X)
  op(0x3d, AbsoluteRead, fp(AND), A, X)
  op(0x3e, AbsoluteModify, fp(ROL), X)
  op(0x40, ReturnInterrupt)
  op(0x41, IndirectXRead, fp(EOR), A)
  op(0x44, NoOperationZeroPage)
  op(0x45, ZeroPageRead, fp(EOR), A)
  op(0x46, ZeroPageModify, fp(LSR))
  op(0x48, Push, A)
  op(0x49, Immediate, fp(EOR), A)
  op(0x4a, Implied, fp(LSR), A)
  op(0x4b, Immediate, fp(ALR), A)
  op(0x4c, JumpAbsolute)
  op(0x4d, AbsoluteRead, fp(EOR), A)
  op(0x4e, AbsoluteModify, fp(LSR))
  op(0x50, Branch, V == 0)
  op(0x51, IndirectYRead, fp(EOR), A)
  op(0x54, NoOperationZeroPage, X)
  op(0x55, ZeroPageRead, fp(EOR), A, X)
  op(0x56, ZeroPageModify, fp(LSR), X)
  op(0x58, Clear, I)
  op(0x59, AbsoluteRead, fp(EOR), A, Y)
  op(0x5a, NoOperation)
  op(0x5c, NoOperationAbsolute, X)
  op(0x5d, AbsoluteRead, fp(EOR), A, X)
  op(0x5e, AbsoluteModify, fp(LSR), X)
  op(0x60, ReturnSubroutine)
  op(0x61, IndirectXRead, fp(ADC), A)
  op(0x64, NoOperationZeroPage)
  op(0x65, ZeroPageRead, fp(ADC), A)
  op(0x66, ZeroPageModify, fp(ROR))
  op(0x68, Pull, A)
  op(0x69, Immediate, fp(ADC), A)
  op(0x6a, Implied, fp(ROR), A)
  op(0x6b, Immediate, fp(ARR), A)
  op(0x6c, JumpIndirect)
  op(0x6d, AbsoluteRead, fp(ADC), A)
  op(0x6e, AbsoluteModify, fp(ROR))
  op(0x70, Branch, V == 1)
  op(0x71, IndirectYRead, fp(ADC), A)
  op(0x74, NoOperationZeroPage, X)
  op(0x75, ZeroPageRead, fp(ADC), A, X)
  op(0x76, ZeroPageModify, fp(ROR), X)
  op(0x78, Set, I)
  op(0x79, AbsoluteRead, fp(ADC), A, Y)
  op(0x7a, NoOperation)
  op(0x7c, NoOperationAbsolute, X)
  op(0x7d, AbsoluteRead, fp(ADC), A, X)
  op(0x7e, AbsoluteModify, fp(ROR), X)
  op(0x80, NoOperationImmediate)
  op(0x81, IndirectXWrite, A)
  op(0x82, NoOperationImmediate)
  op(0x84, ZeroPageWrite, Y)
  op(0x85, ZeroPageWrite, A)
  op(0x86, ZeroPageWrite, X)
  op(0x88, Implied, fp(DEC), Y)
  op(0x89, NoOperationImmediate)
  op(0x8a, Transfer, X, A, 1)
  op(0x8c, AbsoluteWrite, Y)
  op(0x8d, AbsoluteWrite, A)
  op(0x8e, AbsoluteWrite, X)
  op(0x90, Branch, C == 0)
  op(0x91, IndirectYWrite, A)
  op(0x94, ZeroPageWrite, Y, X)
  op(0x95, ZeroPageWrite, A, X)
  op(0x96, ZeroPageWrite, X, Y)
  op(0x98, Transfer, Y, A, 1)
  op(0x99, AbsoluteWrite, A, Y)
  op(0x9a, Transfer, X, S, 0)
  op(0x9d, AbsoluteWrite, A, X)
  op(0xa0, Immediate, fp(LD), Y)
  op(0xa1, IndirectXRead, fp(LD), A)
  op(0xa2, Immediate, fp(LD), X)
  op(0xa4, ZeroPageRead, fp(LD), Y)
  op(0xa5, ZeroPageRead, fp(LD), A)
  op(0xa6, ZeroPageRead, fp(LD), X)
  op(0xa8, Transfer, A, Y, 1)
  op(0xa9, Immediate, fp(LD), A)
  op(0xaa, Transfer, A, X, 1)
  op(0xab, Immediate, fp(ATX), A)
  op(0xac, AbsoluteRead, fp(LD), Y)
  op(0xad, AbsoluteRead, fp(LD), A)
  op(0xae, AbsoluteRead, fp(LD), X)
  op(0xb0, Branch, C == 1)
  op(0xb1, IndirectYRead, fp(LD), A)
  op(0xb3, IndirectYRead, fp(LD), A, X)
  op(0xb4, ZeroPageRead, fp(LD), Y, X)
  op(0xb5, ZeroPageRead, fp(LD), A, X)
  op(0xb6, ZeroPageRead, fp(LD), X, Y)
  op(0xb8, Clear, V)
  op(0xb9, AbsoluteRead, fp(LD), A, Y)
  op(0xba, Transfer, S, X, 1)
  op(0xbc, AbsoluteRead, fp(LD), Y, X)
  op(0xbd, AbsoluteRead, fp(LD), A, X)
  op(0xbe, AbsoluteRead, fp(LD), X, Y)
  op(0xc0, Immediate, fp(CPY), Y)
  op(0xc1, IndirectXRead, fp(CMP), A)
  op(0xc2, NoOperationImmediate)
  op(0xc4, ZeroPageRead, fp(CPY), Y)
  op(0xc5, ZeroPageRead, fp(CMP), A)
  op(0xc6, ZeroPageModify, fp(DEC))
  op(0xc8, Implied, fp(INC), Y)
  op(0xc9, Immediate, fp(CMP), A)
  op(0xca, Implied, fp(DEC), X)
  op(0xcb, Immediate, fp(AXS), X)
  op(0xcc, AbsoluteRead, fp(CPY), Y)
  op(0xcd, AbsoluteRead, fp(CMP), A)
  op(0xce, AbsoluteModify, fp(DEC))
  op(0xd0, Branch, Z == 0)
  op(0xd1, IndirectYRead, fp(CMP), A)
  op(0xd4, NoOperationZeroPage, X)
  op(0xd5, ZeroPageRead, fp(CMP), A, X)
  op(0xd6, ZeroPageModify, fp(DEC), X)
  op(0xd8, Clear, D)
  op(0xd9, AbsoluteRead, fp(CMP), A, Y)
  op(0xda, NoOperation)
  op(0xdc, NoOperationAbsolute, X)
  op(0xdd, AbsoluteRead, fp(CMP), A, X)
  op(0xde, AbsoluteModify, fp(DEC), X)
  op(0xe0, Immediate, fp(CPX), X)
  op(0xe1, IndirectXRead, fp(SBC), A)
  op(0xe2, NoOperationImmediate)
  op(0xe4, ZeroPageRead, fp(CPX), X)
  op(0xe5, ZeroPageRead, fp(SBC), A)
  op(0xe6, ZeroPageModify, fp(INC))
  op(0xe8, Implied, fp(INC), X)
  op(0xe9, Immediate, fp(SBC), A)
  op(0xea, NoOperation)
  op(0xeb, Immediate, fp(SBC), A)
  op(0xec, AbsoluteRead, fp(CPX), X)
  op(0xed, AbsoluteRead, fp(SBC), A)
  op(0xee, AbsoluteModify, fp(INC))
  op(0xf0, Branch, Z == 1)
  op(0xf1, IndirectYRead, fp(SBC), A)
  op(0xf4, NoOperationZeroPage, X)
  op(0xf5, ZeroPageRead, fp(SBC), A, X)
  op(0xf6, ZeroPageModify, fp(INC), X)
  op(0xf8, Set, D)
  op(0xf9, AbsoluteRead, fp(SBC), A, Y)
  op(0xfa, NoOperation)
  op(0xfc, NoOperationAbsolute, X)
  op(0xfd, AbsoluteRead, fp(SBC), A, X)
  op(0xfe, AbsoluteModify, fp(INC), X)
  }

  //unimplemented instruction
  debug(unusual, "[6502] unimplemented opcode ", hex(code));
  return instructionNoOperation();
}

#undef op
#undef fp
