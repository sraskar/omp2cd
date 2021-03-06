#include "bots_main.output.darts.h"
using namespace darts;
using namespace std;
char bots_arg_file[255] = "";
char bots_cc[256];
char bots_cflags[256];
int bots_check_flag = 0;
char bots_comp_date[256];
char bots_comp_message[256];
char bots_cutoff[256];
char bots_exec_date[256];
char bots_exec_message[256];
char bots_execname[256];
char bots_ld[256];
char bots_ldflags[256];
char bots_model[256];
char bots_name[256];
unsigned long long bots_number_of_tasks = 0;
int bots_output_format = 1;
char bots_parameters[256];
int bots_print_header = 0;
char bots_resources[256];
int bots_result = 3;
int bots_sequential_flag = 0;
double bots_time_program = 0.;
double bots_time_sequential = 0.;
bots_verbose_mode_t bots_verbose_mode = BOTS_VERBOSE_DEFAULT;
/*Function: bots_print_usage, ID: 1*/
void bots_print_usage()
{
    /*bots_print_usage:1*/
    /*CompoundStmt:2122*/
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage: %s -[options]\n", bots_execname);
    fprintf(stderr, "\n");
    fprintf(stderr, "Where options are:\n");
    fprintf(stderr, "  -f <file>  : Protein sequences file (mandatory)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  -e <str>   : Include 'str' execution message.\n");
    fprintf(stderr, "  -v <level> : Set verbose level (default = 1).\n");
    fprintf(stderr, "               0 - none.\n");
    fprintf(stderr, "               1 - default.\n");
    fprintf(stderr, "               2 - debug.\n");
    fprintf(stderr, "  -o <value> : Set output format mode (default = 1).\n");
    fprintf(stderr, "               0 - no benchmark output.\n");
    fprintf(stderr, "               1 - detailed list format.\n");
    fprintf(stderr, "               2 - detailed row format.\n");
    fprintf(stderr, "               3 - abridged list format.\n");
    fprintf(stderr, "               4 - abridged row format.\n");
    fprintf(stderr, "  -z         : Print row header (if output format is a row variant).\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  -s         : Run sequential version.\n");
    fprintf(stderr, "  -c         : Check mode ON (implies running sequential version).\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  -h         : Print program's usage (this help).\n");
    fprintf(stderr, "\n");
}
/*Function: bots_get_params_common, ID: 2*/
void bots_get_params_common(int argc, char** argv)
{
    /*bots_get_params_common:2*/
    /*CompoundStmt:2147*/
    int i;
    strcpy(bots_execname, __xpg_basename(argv[0]));
    bots_get_date(bots_exec_date);
    strcpy(bots_exec_message, "");
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'c':
                argv[i][1] = '*';
                bots_check_flag = 1;
                break;
            case 'e':
                argv[i][1] = '*';
                i++;
                if (argc == i) {
                    bots_print_usage();
                    exit(100);
                }
                strcpy(bots_exec_message, argv[i]);
                break;
            case 'f':
                argv[i][1] = '*';
                i++;
                if (argc == i) {
                    bots_print_usage();
                    exit(100);
                }
                strcpy(bots_arg_file, argv[i]);
                break;
            case 'h':
                argv[i][1] = '*';
                bots_print_usage();
                exit(100);
            case 'o':
                argv[i][1] = '*';
                i++;
                if (argc == i) {
                    bots_print_usage();
                    exit(100);
                }
                bots_output_format = atoi(argv[i]);
                break;
            case 's':
                argv[i][1] = '*';
                bots_sequential_flag = 1;
                break;
            case 'v':
                argv[i][1] = '*';
                i++;
                if (argc == i) {
                    bots_print_usage();
                    exit(100);
                }
                bots_verbose_mode = (bots_verbose_mode_t)atoi(argv[i]);
                if (bots_verbose_mode > 1) {
                    fprintf(stderr, "Error: Configure the suite using '--debug' option in order to "
                                    "use a verbose level greather than 1.\n");
                    exit(100);
                }
                break;
            case 'z':
                argv[i][1] = '*';
                bots_print_header = 1;
                break;
            default:
                fprintf(stderr, "Error: Unrecognized parameter.\n");
                bots_print_usage();
                exit(100);
            }
        } else {
            fprintf(stderr, "Error: Unrecognized parameter.\n");
            bots_print_usage();
            exit(100);
        }
    }
}
/*Function: bots_get_params, ID: 3*/
void bots_get_params(int argc, char** argv)
{
    /*bots_get_params:3*/
    /*CompoundStmt:2274*/
    bots_get_params_common(argc, argv);
}
/*Function: bots_set_info, ID: 4*/
void bots_set_info()
{
    /*bots_set_info:4*/
    /*CompoundStmt:2276*/
    snprintf(bots_name, 256, "Protein alignment (Single version)");
    snprintf(bots_parameters, 256, "%s", bots_arg_file);
    snprintf(bots_model, 256, "OpenMP (using tasks)");
    snprintf(bots_resources, 256, "%d", 1);
    strcpy(bots_cutoff, "none");
}
/*Function: main, ID: 5*/
int main(int argc, char* argv[])
{
    getOMPNumThreads();
    getOMPSchedulePolicy();
    getTPLoopThresholds();
    getNumTPs();
    affin = new ThreadAffinity(
        ompNumThreads / NUMTPS - 1, NUMTPS, COMPACT, getDARTSTPPolicy(), getDARTSMCPolicy());
    affinMaskRes = affin->generateMask();
    myDARTSRuntime = new Runtime(affin);
    RuntimeFinalCodelet = &(myDARTSRuntime->finalSignal);
    /*main:5*/
    /*CompoundStmt:2282*/
    long bots_t_start;
    long bots_t_end;
    bots_get_params(argc, argv);
    pairalign_init(bots_arg_file);
    bots_set_info();
    if (bots_sequential_flag || bots_check_flag) {
        bots_sequential_flag = 1;
        align_seq_init();
        bots_t_start = bots_usecs();
        align_seq();
        bots_t_end = bots_usecs();
        bots_time_sequential = ((double)(bots_t_end - bots_t_start)) / 1000000;
        ;
    }
    align_init();
    bots_t_start = bots_usecs();
    align();
    bots_t_end = bots_usecs();
    bots_time_program = ((double)(bots_t_end - bots_t_start)) / 1000000;
    align_end();
    if (bots_check_flag) {
        bots_result = align_verify();
    }
    bots_print_results();
    return (0);
}
