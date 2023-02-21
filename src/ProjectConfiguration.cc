#include "champsim.h"

OutputMemoryTraceFileType outputmemorytrace_one;

OutputChampSimStatisticsFileType outputchampsimstatistics;

void output_memory_trace_initialization(const char* string)
{
    char* string2 = (char*)malloc(strlen(string) + 1);
    strcpy(string2, string);

    const char* delimiter = "/";
    char* token = NULL;
    char* last_token = NULL;

    /* get the first token */
    token = strtok(string2, delimiter);

    /* walk through other tokens */
    while (token != NULL)
    {
        last_token = token;
        token = strtok(NULL, delimiter);
    }

    // append file_extension to string3.
    const char* file_extension = ".trace";
    char* string3 = (char*)malloc(strlen(last_token) + 1 + strlen(file_extension));
    strcpy(string3, last_token);
    strcat(string3, file_extension);

    outputmemorytrace_one.trace_file = fopen(string3, "w");
    outputmemorytrace_one.trace_string = (char*)malloc(strlen(string3) + 1);
    strcpy(outputmemorytrace_one.trace_string, string3);
}

void output_memory_trace_deinitialization(OutputMemoryTraceFileType& outputmemorytrace)
{
    fclose(outputmemorytrace.trace_file);
    printf("Output memory trace into %s.\n", outputmemorytrace.trace_string);

    free(outputmemorytrace.trace_string);
}

void output_memory_trace_hexadecimal(OutputMemoryTraceFileType& outputmemorytrace, uint64_t address, char type)
{
    fprintf(outputmemorytrace.trace_file, "0x%lx %c\n", address, type);
}

void output_champsim_statistics_initialization(const char* string)
{
    char* string2 = (char*)malloc(strlen(string) + 1);
    strcpy(string2, string);

    const char* delimiter = "/";
    char* token;
    char* last_token;

    /* get the first token */
    token = strtok(string2, delimiter);

    /* walk through other tokens */
    while (token != NULL)
    {
        last_token = token;
        token = strtok(NULL, delimiter);
    }

    // append file_extension to string3.
    const char* file_extension = ".statistics";
    char* string3 = (char*)malloc(strlen(last_token) + 1 + strlen(file_extension));
    strcpy(string3, last_token);
    strcat(string3, file_extension);

    outputchampsimstatistics.trace_file = fopen(string3, "w");
    outputchampsimstatistics.trace_string = (char*)malloc(strlen(string3) + 1);
    strcpy(outputchampsimstatistics.trace_string, string3);

    /* Initialize variabes */
    for (uint64_t i = 0; i < outputchampsimstatistics.valid_pte_count.size(); i++)
    {
        outputchampsimstatistics.valid_pte_count[i] = 0;
    }
    outputchampsimstatistics.virtual_page_count = 0;

    outputchampsimstatistics.read_request_in_memory = 0;
    outputchampsimstatistics.read_request_in_memory2 = 0;
    outputchampsimstatistics.write_request_in_memory = 0;
    outputchampsimstatistics.write_request_in_memory2 = 0;

    outputchampsimstatistics.swapping_count = 0;
    outputchampsimstatistics.swapping_traffic_in_bytes = 0;

    outputchampsimstatistics.remapping_request_queue_congestion = 0;

#if (IDEAL_VARIABLE_GRANULARITY == ENABLE)
    outputchampsimstatistics.no_free_space_for_migration = 0;
    outputchampsimstatistics.no_invalid_group_for_migration = 0;
    outputchampsimstatistics.unexpandable_since_start_address = 0;
    outputchampsimstatistics.unexpandable_since_no_invalid_group = 0;
    outputchampsimstatistics.data_eviction_success = outputchampsimstatistics.data_eviction_failure = 0;
    outputchampsimstatistics.uncertain_counter = 0;
#endif  // IDEAL_VARIABLE_GRANULARITY
}

void output_champsim_statistics_deinitialization(OutputChampSimStatisticsFileType& outputchampsimstatistics)
{
    fprintf(outputchampsimstatistics.trace_file, "\n\nInformation about virtual memory\n\n");
    for (uint64_t i = 0; i < outputchampsimstatistics.valid_pte_count.size(); i++)
    {
        fprintf(outputchampsimstatistics.trace_file, "Level: %ld, valid_pte_count: %ld.\n", i, outputchampsimstatistics.valid_pte_count[i]);
    }
    fprintf(outputchampsimstatistics.trace_file, "virtual_page_count: %ld, main memory footprint: %f MB.\n", outputchampsimstatistics.virtual_page_count, outputchampsimstatistics.virtual_page_count * 4.0 / KB);

    uint64_t total_access_request_in_memory = outputchampsimstatistics.read_request_in_memory + outputchampsimstatistics.read_request_in_memory2 + outputchampsimstatistics.write_request_in_memory + outputchampsimstatistics.write_request_in_memory2;

    fprintf(outputchampsimstatistics.trace_file, "\n\nInformation about memory controller\n\n");
    fprintf(outputchampsimstatistics.trace_file, "read_request_in_memory: %ld, read_request_in_memory2: %ld.\n", outputchampsimstatistics.read_request_in_memory, outputchampsimstatistics.read_request_in_memory2);
    fprintf(outputchampsimstatistics.trace_file, "write_request_in_memory: %ld, write_request_in_memory2: %ld.\n", outputchampsimstatistics.write_request_in_memory, outputchampsimstatistics.write_request_in_memory2);
    fprintf(outputchampsimstatistics.trace_file, "hit rate: %f.\n", (outputchampsimstatistics.read_request_in_memory + outputchampsimstatistics.write_request_in_memory) / float(total_access_request_in_memory));

    fprintf(outputchampsimstatistics.trace_file, "swapping_count: %ld, swapping_traffic_in_bytes: %ld.\n", outputchampsimstatistics.swapping_count, outputchampsimstatistics.swapping_traffic_in_bytes);

    fprintf(outputchampsimstatistics.trace_file, "remapping_request_queue_congestion: %ld.\n", outputchampsimstatistics.remapping_request_queue_congestion);

#if (IDEAL_VARIABLE_GRANULARITY == ENABLE)
    fprintf(outputchampsimstatistics.trace_file, "no_free_space_for_migration: %ld (%f).\n", outputchampsimstatistics.no_free_space_for_migration, outputchampsimstatistics.no_free_space_for_migration / float(total_access_request_in_memory));
    fprintf(outputchampsimstatistics.trace_file, "no_invalid_group_for_migration: %ld (%f).\n", outputchampsimstatistics.no_invalid_group_for_migration, outputchampsimstatistics.no_invalid_group_for_migration / float(total_access_request_in_memory));
    fprintf(outputchampsimstatistics.trace_file, "unexpandable_since_start_address: %ld (%f).\n", outputchampsimstatistics.unexpandable_since_start_address, outputchampsimstatistics.unexpandable_since_start_address / float(total_access_request_in_memory));
    fprintf(outputchampsimstatistics.trace_file, "unexpandable_since_no_invalid_group: %ld (%f).\n", outputchampsimstatistics.unexpandable_since_no_invalid_group, outputchampsimstatistics.unexpandable_since_no_invalid_group / float(total_access_request_in_memory));
    fprintf(outputchampsimstatistics.trace_file, "data_eviction_success: %ld (%f).\n", outputchampsimstatistics.data_eviction_success, outputchampsimstatistics.data_eviction_success / float(total_access_request_in_memory));
    fprintf(outputchampsimstatistics.trace_file, "data_eviction_failure: %ld (%f).\n", outputchampsimstatistics.data_eviction_failure, outputchampsimstatistics.data_eviction_failure / float(total_access_request_in_memory));
    fprintf(outputchampsimstatistics.trace_file, "uncertain_counter: %ld (%f).\n", outputchampsimstatistics.uncertain_counter, outputchampsimstatistics.uncertain_counter / float(total_access_request_in_memory));
#endif  // IDEAL_VARIABLE_GRANULARITY

    fclose(outputchampsimstatistics.trace_file);
    printf("Output ChampSim statistics into %s.\n", outputchampsimstatistics.trace_string);

    free(outputchampsimstatistics.trace_string);
}
