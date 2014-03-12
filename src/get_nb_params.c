/*
** get_nb_params.c for  in /home/bartholomew/programmation/c/prototype
**
** Made by Bartholomew
** Bartholomew de la Villardiere <exabot@gmail.com>
**
** Started on  mer. mars 12 04:30:58 2014
** Last update mer. mars 12 04:30:58 2014
*/

#include <inttypes.h>
#include <capstone/capstone.h>
#include <string.h>

typedef enum
{
    not_used_in_fct = 0,
    not_set_in_fct,
    set_in_fct
} t_state;

typedef enum
{
    src_op = 0,
    dst_op
} t_position;

static void update_register_state(
        t_state* regs,
        cs_x86_op* op,
        t_position position,
        unsigned int* nb_params)
{
    if (position == src_op)
    {
        if (regs[op->reg] == not_used_in_fct && op->reg != X86_REG_RSP)
        {
            regs[op->reg] = not_set_in_fct;
            ++(*nb_params);
        }
    }
    else
        regs[op->reg] = set_in_fct;
}

int get_nb_params(const uint8_t* fct)
{
    csh handle;
    cs_insn* insn = 0;
    cs_x86* x86 = 0;
    cs_x86_op* op = 0;
    t_state regs[X86_REG_MAX] = {not_used_in_fct};
    size_t count = 0;
    size_t i = 0;
    unsigned int nb_params = 0;
    unsigned char stop = 0;

    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK)
        return (-1);
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
    while (!stop)
    {
        count = cs_disasm_ex(handle, fct, 100, 0, 0, &insn);
        for (i = 0; i < count && !stop; ++i)
            if (strncmp(insn[i].mnemonic, "ret", 3) == 0)
                stop = 1;
            else
            {
                x86 = &(insn[i].detail->x86);
                if (x86->op_count >= 2)
                {
                    op = x86->operands + 1;
                    if ((int)op->type == X86_OP_REG)
                        update_register_state(regs, op, src_op, &nb_params);
                    op = x86->operands;
                    if ((int)op->type == X86_OP_REG)
                        update_register_state(regs, op, dst_op, &nb_params);
                }
            }
    }
    cs_close(&handle);
    return (nb_params);
}
