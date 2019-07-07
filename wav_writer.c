#include <stdio.h>
#include "C:\\CPP\\wav_reader\\wav_reader.h"
void write_string(FILE *fp, char *input_string, int chars_to_write)
{
	int i;
	for (i = 0; i < chars_to_write; i++)
	{
		putc(input_string[i], fp);
	}
}

/* This should take a bit size as an argument, then appropriately write the number little-endian */
/* Actually, just assume a bit depth of 16 since WaveFile has short int as its storage data type */
void write_number(FILE *fp, long int input_number, int ints_to_write)
{
	int output_char = 0;
	int i;
//printf("writing %d bytes of number %ld\n", ints_to_write, input_number);
	for (i = 0; i < ints_to_write; i++)
	{
		output_char = input_number >> (i * 8);
		output_char &= 0xFF;
//printf("writing char %d\n", output_char);
		putc(output_char, fp);
	}
}

void write_data_chunk(FILE *fp, short input_number, int num_bytes_to_write)
{
	int i;
	int output;
//	printf("Writing number %d\n", input_number);
	for (i = 0; i < num_bytes_to_write; i++)
	{
		output = input_number >> (i * 8);
		output &= 0xFF;
//		printf("Writing char %d\n", output);
		putc(output, fp);
	}
}

void write_wave(WaveFile wavefile, char *filename)
{
	FILE *fp = fopen(filename, "wb"); // Double check open type

	int i,j;

	printf("Writing to file %s\n", filename);

	write_string(fp, wavefile.riff_marker, 4); // Should this just be hardcoded?
	write_number(fp, wavefile.file_size, 4); // Need to calculate this
	write_string(fp, wavefile.file_type_header, 4);
	write_string(fp, wavefile.format_chunk_marker, 4);
	write_number(fp, wavefile.format_data_length, 4);
	write_number(fp, wavefile.format_type, 2);
	write_number(fp, wavefile.num_channels, 2);
	write_number(fp, wavefile.sample_rate, 4);
	write_number(fp, wavefile.byterate, 4);
	write_number(fp, wavefile.bitrate_math, 2);
	write_number(fp, wavefile.bits_per_sample, 2);
	write_string(fp, wavefile.data_chunk_header, 4);
	write_number(fp, wavefile.data_section_size, 4);

	printf("Writing %ld frames\n", wavefile.num_frames);
	/* Now write all the data */
	for (i = 0; i < wavefile.num_frames; i++)
	{
//		printf("Writing frame %d\n", i);
		for (j = 0; j < wavefile.num_channels; j++)
		{
//			printf("Writing channel %d\n", j);
			write_data_chunk(fp, wavefile.channel_samples[j][i], wavefile.bits_per_sample / 8);
		}
	}

	fclose(fp);
}

#if 0
//#include "C:\\CPP\\wav_reader\\wav_reader.h"
int main()
{
	WaveFile wavefile = { 0 };
	WaveFile wavefile_output = { 0 };
	char filename[128] = "C:\\Users\\PC\\Downloads\\WAV_Samples\\M1F1-int16-AFsp.wav";
	char output_filename[128] = "output_test.wav";

	printf("\nInput file\n");
	printf("----------\n");
	read_wave(&wavefile, filename);
	print_header(wavefile);

	write_wave(wavefile, output_filename);
	destroy_wavearrays(&wavefile);
	
	read_wave(&wavefile_output, output_filename);
	printf("\nOutput file, read in again:\n");
	printf("-----------\n");
	print_header(wavefile_output);
	destroy_wavearrays(&wavefile_output);

	return 0;
} 
#endif
