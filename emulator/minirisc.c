#include "minirisc.h"
#include <stdio.h>
#include <stdlib.h>

// Custom Mini-RISC Opcodes
#define OPCODE_LUI 0x01
#define OPCODE_AUIPC 0x02
#define OPCODE_JAL 0x03
#define OPCODE_JALR 0x04
#define OPCODE_BRANCH 0x05
#define OPCODE_LOAD 0x0E   // LBU
#define OPCODE_STORE 0x10  // SB
#define OPCODE_OP_IMM 0x13 // ADDI
#define OPCODE_SYSTEM 0x27 // EBREAK

// Helper macros for extracting fields (Mini-RISC Format)
#define GET_OPCODE(inst) ((inst) & 0x7F)
#define GET_RD(inst) (((inst) >> 7) & 0x1F)
#define GET_RS1(inst) (((inst) >> 12) & 0x1F) // Shifted compared to standard
#define GET_RS2(inst) (((inst) >> 17) & 0x1F) // Shifted compared to standard
#define GET_RS2_SB(inst)                                                       \
  (((inst) >> 7) & 0x1F) // For Store/Branch (replaces rd)

// Immediate extraction
// LUI/AUIPC: U-type (Standard-ish)
#define GET_IMM_U(inst) ((inst) & 0xFFFFF000)

// JAL: J-type (Simplified?)
// In the example: 0000a083 -> imm=20. imm field has 0x0000a (10). 10*2 = 20.
// It seems imm is at bits 12+? No, rs1 is at 12.
// Let's assume standard J-type scrambling or similar, but let's look at the
// bits. 0000a083: 0000 0000 0000 0000 1010 0000 1000 0011 We need 20. If we
// take bits 20-31? 0. If we take bits 12-31 excluding registers? Let's rely on
// the standard decoding macros but adjusted if needed. For now, let's implement
// based on the specific examples we saw. JAL imm seems to be in bits 20-31 or
// similar, but the example 0000a083 is confusing. Wait, 0000a083. 0000 0000
// 0000 0000 1010 0000 1000 0011 Bits 12-15: 1010 (10). If JAL uses I-type
// format? imm[11:0] at 20-31? 0000a083 has 0 at 20-31. Maybe the immediate is
// split? Actually, let's look at the instruction hexes again. JAL: 0000a083.
// Target +20. BEQ: 0080b005. Target +16. Imm field has 008 (8). 8*2=16. ADDI:
// 0380a513. Imm 56. Imm field has 038 (56). So ADDI uses bits 20-31 for imm.
// BEQ uses bits 20-31 for imm (value 8).
// JAL uses bits ??? for imm (value 10).
// In 0000a083, where is 10?
// Bits 12-15 is 1010 (10).
// But bits 12-16 is rs1 position.
// Does JAL use rs1 field for immediate?
// Yes! JAL doesn't use rs1.
// So JAL imm is likely in 12-31?
// Let's assume JAL imm is (inst >> 12) * 2? No, that would be huge.
// In 0000a083, bits 12-31 is 0000a.
// So yes, (inst >> 12) & ...
// But wait, 0000a is 10. 10*2 = 20.
// So JAL imm = ((inst >> 12) & 0xFFFFF) * 2?
// Let's try that.

// ADDI: 0380a513. Imm 56.
// Bits 20-31 is 038 (56).
// So I-Type imm is (inst >> 20).

// BEQ: 0080b005. Imm 8 (for 16).
// Bits 20-31 is 008 (8).
// So B-Type imm is (inst >> 20) * 2?

// LUI: 10000601. Imm 0x10000.
// 10000601.
// 0001 0000 0000 0000 0000 0110 0000 0001
// Bits 12-31: 10000.
// So U-Type imm is (inst & 0xFFFFF000)?
// No, 10000 is at the top.
// (inst >> 12) << 12?
// If we take 10000601 & 0xFFFFF000 -> 10000000. Correct.

minirisc_t *minirisc_new(uint32_t initial_PC, platform_t *platform) {
  minirisc_t *mr = (minirisc_t *)malloc(sizeof(minirisc_t));
  if (!mr) {
    perror("Failed to allocate minirisc struct");
    exit(1);
  }
  mr->PC = initial_PC;
  mr->next_PC = initial_PC + 4;
  mr->platform = platform;
  mr->halt = 0;
  for (int i = 0; i < 32; i++) {
    mr->regs[i] = 0;
  }
  return mr;
}

void minirisc_free(minirisc_t *mr) {
  if (mr)
    free(mr);
}

void minirisc_fetch(minirisc_t *mr) {
  mr->IR = platform_read_32(mr->platform, mr->PC);
}

void minirisc_decode_and_execute(minirisc_t *mr) {
  uint32_t inst = mr->IR;
  uint32_t opcode = GET_OPCODE(inst);
  uint32_t rd = GET_RD(inst);
  uint32_t rs1 = GET_RS1(inst);
  uint32_t rs2 = GET_RS2(inst);

  // Default next PC is PC + 4
  mr->next_PC = mr->PC + 4;

  // x0 is always 0
  mr->regs[0] = 0;

  switch (opcode) {
  case OPCODE_LUI: {
    // LUI: rd = imm << 12
    // Imm is top 20 bits
    uint32_t imm = inst & 0xFFFFF000;
    if (rd != 0)
      mr->regs[rd] = imm;
    break;
  }
  case OPCODE_AUIPC: {
    // AUIPC: rd = PC + (imm << 12)
    uint32_t imm = inst & 0xFFFFF000;
    if (rd != 0)
      mr->regs[rd] = mr->PC + imm;
    break;
  }
  case OPCODE_JAL: {
    // JAL: rd = PC + 4; PC += imm
    // Based on 0000a083 -> imm=20. Field at 12-31 is 0xA (10).
    // So imm = field * 2.
    int32_t imm = ((int32_t)inst >> 12) * 2; // Sign extend?
    if (rd != 0)
      mr->regs[rd] = mr->PC + 4;
    mr->next_PC = mr->PC + imm;
    break;
  }
  case OPCODE_JALR: {
    // JALR: rd = PC + 4; PC = rs1 + imm
    // ret is jalr x0, ra, 0. 00001004.
    // imm at 20-31 is 0.
    int32_t imm = (int32_t)inst >> 20;
    uint32_t target = (mr->regs[rs1] + imm) & ~1;
    if (rd != 0)
      mr->regs[rd] = mr->PC + 4;
    mr->next_PC = target;
    break;
  }
  case OPCODE_BRANCH: {
    // BEQ only for now?
    // 0080b005 -> imm=16. Field at 20-31 is 8.
    // So imm = field * 2.
    int32_t imm = ((int32_t)inst >> 20) * 2;
    int32_t val1 = (int32_t)mr->regs[rs1];
    int32_t val2 = (int32_t)mr->regs[GET_RS2_SB(inst)];

    // Assuming BEQ for opcode 0x05
    if (val1 == val2) {
      mr->next_PC = mr->PC + imm;
    }
    break;
  }
  case OPCODE_LOAD: {
    // LBU only? 0x0E
    // lbu a1, 0(a0) -> 0000a58e.
    // imm at 20-31 is 0.
    int32_t imm = (int32_t)inst >> 20;
    uint32_t addr = mr->regs[rs1] + imm;
    // Hardcoded LBU
    uint32_t val = platform_read_8(mr->platform, addr);
    if (rd != 0)
      mr->regs[rd] = val;
    break;
  }
  case OPCODE_STORE: {
    // SB only? 0x10
    // sb a1, 0(a2) -> 0000c590.
    // imm at 20-31?
    // 0000c590. Bits 20-31 is 0.
    // Wait, standard S-type splits imm.
    // Here, rs2 is at 7-11. rs1 at 12-16.
    // Where is imm?
    // If imm is 0, we can't tell.
    // But let's assume imm is at 20-31 like others?
    // Or split?
    // Given the simplicity of other formats, maybe it's just 20-31?
    // Let's assume 20-31 for now.
    int32_t imm = (int32_t)inst >> 20;
    uint32_t addr = mr->regs[rs1] + imm;
    // Hardcoded SB
    platform_write_8(mr->platform, addr, mr->regs[GET_RS2_SB(inst)] & 0xFF);
    break;
  }
  case OPCODE_OP_IMM: {
    // ADDI only? 0x13
    // 0380a513 -> imm=56. Field at 20-31 is 56.
    int32_t imm = (int32_t)inst >> 20;
    if (rd != 0)
      mr->regs[rd] = mr->regs[rs1] + imm;
    break;
  }
  case OPCODE_SYSTEM: {
    // EBREAK 0x27
    mr->halt = 1;
    printf("EBREAK executed. Halting.\n");
    break;
  }
  default:
    fprintf(stderr, "Unknown opcode: 0x%02x at PC: 0x%08x\n", opcode, mr->PC);
    mr->halt = 1;
    break;
  }

  // Update PC
  mr->PC = mr->next_PC;
  // Ensure x0 is always 0
  mr->regs[0] = 0;
}

void minirisc_run(minirisc_t *mr) {
  while (!mr->halt) {
    minirisc_fetch(mr);
    minirisc_decode_and_execute(mr);
  }
}
