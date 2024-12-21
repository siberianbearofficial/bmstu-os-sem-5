/*
* filename: service.x
 * function: Define constants, non-standard data types and the calling
process in remote calls
*/
const ADD = 0;
const SUB = 1;
const MUL = 2;
const DIV = 3;

struct REQUEST
{
    int number;
    int op;
    double arg1;
    double arg2;
};

program SERVICE_PROG
    {
        version SERVICE_VER
    {
        double SERVICE_PROC(struct REQUEST) = 1;
        int NUMBER_PROC(void) = 2;
    } = 1; /* Version number = 1 */
    }
= 0x20000001; /* RPC program number */
