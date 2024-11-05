// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <std.h>
#include <syscall_adapters.h>
#include <stdarg.h>

uint32_t strcmp(const char* s1, const char* s2) {
    uint32_t i;
    for (i = 0; s1[i] != '\0' && s2[i] != '\0'; i++)
        if (s1[i] != s2[i])
            return s1[i] - s2[i];

    if (s1[i] == '\0' && s2[i] == '\0') return 0;

    return s1[i] - s2[i];
}

void strcpy(char* dest, const char* src) {
    uint32_t i;

    for (i = 0; src[i] != '\0'; i++)
        dest[i] = src[i];

    dest[i] = '\0';
}

void strcat(char* dest, const char* str1, const char * str2) {
    uint32_t i;
    uint32_t j;

    for (i = 0; str1[i] != '\0'; i++)
        dest[i] = str1[i];

    for (j = 0; str2[j] != '\0'; j++)
        dest[i + j] = str2[j];

    dest[i + j] = '\0';
}

int split(const char* source, char separator, char* dest[], int dest_qty) {
    int i = 0;
    int j = 0;
    int k = 0;
    int new_string = 0;

    while (source[i] != '\0' && j < dest_qty) {
        if (source[i] == separator) {
            if (new_string) {
                dest[j][k] = '\0';
                j++;
                k = 0;
                new_string = 0;
            }
        } else {
            dest[j][k] = source[i];
            k++;
            new_string = 1;
        }
        i++;
    }

    if (new_string) {
        dest[j][k] = '\0';
        j++;
    }

    return j;
}

uint32_t strlen(const char* s) {
    uint32_t i;

    for (i = 0; s[i] != '\0'; i++) ;

    return i;
}

void putchar(char c) {
    sys_write(STDOUT, &c, 1, 1);
}

void puts(const char* s) {
    sys_write(STDOUT, s, strlen(s), 0x00ffffff);
}

void puts_with_color(const char* s, uint32_t hexColor) {
    sys_write(STDOUT, s, strlen(s), hexColor);
}

char* itoa(uint64_t num, char* dest, uint32_t dest_max_len) {
    if (dest_max_len == 0) {
        return dest;
    }

    if (num == 0) {
        dest[0] = '0';
        dest[1] = '\0';
        return dest;
    }

    char itoa_buff[ITOA_BUFF_MAX_SIZE];
    uint32_t i = 0;

    while (num != 0 && i < ITOA_BUFF_MAX_SIZE) {
        itoa_buff[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (i >= dest_max_len) {
        i = dest_max_len - 1;
    }

    uint32_t j = 0;
    while (j < i) {
        dest[j] = itoa_buff[i - j - 1];
        j++;
    }

    dest[j] = '\0';

    return dest;
}


uint32_t get_fmt_num_args(const char* fmt) {
    uint32_t count = 0;
    for (uint32_t i = 0; fmt[i] != '\0'; i++) {
        count += (fmt[i] == '%' && fmt[i + 1] == 's');
        count += (fmt[i] == '%' && fmt[i + 1] == 'd');
        count += (fmt[i] == '%' && fmt[i + 1] == 'c');
    }
    return count;
}

void printf(const char* fmt, ...) {
    char printf_buff[PRINTF_PRINT_BUFF_MAX_SIZE];

    va_list arg_list;
    va_start(arg_list, fmt);  // Corregido el argumento de va_start

    uint32_t k = 0;
    for (uint32_t i = 0; k < PRINTF_PRINT_BUFF_MAX_SIZE && fmt[i] != '\0'; i++) {
        if (fmt[i] == '%' && fmt[i + 1] == 's') {
            char* arg_s = va_arg(arg_list, char*);
            for (uint32_t j = 0; k < PRINTF_PRINT_BUFF_MAX_SIZE && arg_s[j] != '\0'; j++)
                printf_buff[k++] = arg_s[j];
            i++;  // Saltar el 's'
        }
        else if (fmt[i] == '%' && fmt[i + 1] == 'c') {
            printf_buff[k++] = (char)va_arg(arg_list, int);  // Convertir a char correctamente
            i++;  // Saltar el 'c'
        }
        else if (fmt[i] == '%' && fmt[i + 1] == 'd') {
            int32_t arg_d = va_arg(arg_list, int32_t);  // Cambiado a int32_t para manejar números con signo
            char temp[ITOA_BUFF_MAX_SIZE];
            
            // Manejar enteros negativos
            if (arg_d < 0) {
                if (k < PRINTF_PRINT_BUFF_MAX_SIZE) {
                    printf_buff[k++] = '-';
                }
                arg_d = -arg_d;
            }
            
            itoa(arg_d, temp, ITOA_BUFF_MAX_SIZE);

            for (uint32_t j = 0; k < PRINTF_PRINT_BUFF_MAX_SIZE && temp[j] != '\0'; j++)
                printf_buff[k++] = temp[j];

            i++;  // Saltar el 'd'
        }
        else if (fmt[i] == '%' && fmt[i + 1] == '%') {
            if (k < PRINTF_PRINT_BUFF_MAX_SIZE) {
                printf_buff[k++] = '%';  // Manejar "%%" como un solo '%'
            }
            i++;  // Saltar el segundo '%'
        }
        else {
            printf_buff[k++] = fmt[i];  // Copiar otros caracteres literalmente
        }
    }
    printf_buff[k] = '\0';  // Terminar la cadena

    va_end(arg_list);

    puts(printf_buff);
}


char getchar() {
    return sys_get_character_pressed();
}

uint32_t atoi(const char* s) {
    uint32_t out = 0;

    for (uint32_t i = 0; s[i] != '\0'; i++)
        out = out * 10 + (s[i] - '0');

    return out;
}

uint64_t scanf(const char* fmt, void* ptr) {
    char scan_buff[SCANF_BUFF_MAX_SIZE] = {0};

    uint32_t len = sys_read(STDIN, scan_buff, SCANF_BUFF_MAX_SIZE);
    if (len > 0){
        scan_buff[len - 1] = '\0';

        if (strcmp(fmt, "%d") == 0)
            *((uint32_t*)ptr) = atoi(scan_buff);
        else if (strcmp(fmt, "%s") == 0)
            strcpy((char*)ptr, scan_buff);
        else if (strcmp(fmt, "%c") == 0)
            *((char*)ptr) = scan_buff[0];
    }
    return len;
}

int isAlpha(char c){
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9');
}

void * memset(void * destination, int32_t c, uint64_t length){
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length){
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}