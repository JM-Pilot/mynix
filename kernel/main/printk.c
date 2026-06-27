#include <drivers/console/console.h>
#include <sys/kernel.h>
#include <sys/printk.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
/* this function is self explainatory */
/* If you dont know how this works, read it ;) (not mean) */

/* Ultra long function,
 * if base == 10, then you can skip has_prefix and is_uppercase
 * they are only reserved for the hex and binary bases
 */
char *num_to_ascii(
	char *buf, 
	int64_t val, 
	int base, 
	int precision, 
	bool is_unsigned, 
	bool has_prefix,
	bool is_uppercase
)
{
	int i, j;
	uint64_t uval = (uint64_t)val;
	i = 0;
	j = 0;
	if (base == 10){
		char tmp[21];
		bool negative = false;
		if (is_unsigned){
			do {
				tmp[i++] = uval % 10 + '0';
			} while(uval /= 10);		

			while (i--)
				buf[j++] = tmp[i];
			buf[j] = '\0';
			return buf;
		}
		if (val < 0) {
			negative = true;
			val = -val;
		}
		do {
			tmp[i++] = val % 10 + '0';
		} while(val /= 10);
		
		if (negative) 
			tmp[i++] = '-';

		while (i--)
			buf[j++] = tmp[i];
		buf[j] = '\0';
		return buf;
	}
	else if (base == 16) {
		char *hex;
		if (is_uppercase)
			hex = "0123456789ABCDEF";
		else
			hex = "0123456789abcdef";
		char tmp[18];

		i = 0;
		j = 0;
		while (uval) {
			tmp[i++] = hex[uval & 0xF];
			uval >>= 4;
		}

		while (i < precision) 
			tmp[i++] = '0';

		if (has_prefix) {
			tmp[i++] = 'x';
			tmp[i++] = '0';
		}

		while (i--)
			buf[j++] = tmp[i];
		buf[j] = '\0';
		return buf;
	}
	else if (base == 2) {
		char tmp[66];
		i = 0;
		j = 0;
		while (uval) {
			tmp[i++] = '0' + (uval & 0x1);
			uval >>= 1;
		}
		
		while (i < precision) 
			tmp[i++] = '0';

		if (has_prefix) {
			tmp[i++] = 'b';
			tmp[i++] = '0';
		}


		while (i--)
			buf[j++] = tmp[i];
		buf[j] = '\0';
		return buf;
	}
	buf[j] = '\0';
	return buf;
}
bool is_a_num(char a)
{
	return (a >= '0' && a <= '9');
}
int ascii_to_num(int dest, char *src_buf, int *cnt)
{
	while (is_a_num(*src_buf)) {
		dest = dest * 10 + (*src_buf - '0');
		src_buf++;
		(*cnt)++;
	}
	return dest;
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
	int count = 0;
	int precision = 0;
	bool prefix = false;
	bool uppercase = false;
	while (*fmt) {
		if (*fmt == '%') {
			fmt++;
			/* Check if we want to add a prefix (0b 0x)*/
			if (*fmt == '#') {
				fmt++;
				prefix = true;
			}
			/* Get The precision */
			if (*fmt == '.') {
				fmt++;
				int tmp = 0;
				precision = ascii_to_num(precision, (char *)fmt, &tmp);
				fmt += tmp;
			}
			switch (*fmt)
			{
				case 'c':
					*buf++ = (char)va_arg(args, int);
					break;
				case 's': {
					char *s = va_arg(args, char *);
					while (*s)
						*buf++ = *s++;
					break;
				}
				case 'd': {
					char tmp[21];
					num_to_ascii(tmp, va_arg(args, int) 
					, 10, 0, false, false, false);
					for (int i = 0; tmp[i] != '\0'; i++)
						*buf++ = tmp[i];
					break;
				}
				case 'u': {
					char tmp[21];
					num_to_ascii(tmp, va_arg(args, uint32_t), 10, 0, true, false, false);
					for (int i = 0; tmp[i] != '\0'; i++)
						*buf++ = tmp[i];
					break;
				}
				/* These fallthrough attributes silence the compiler */
				case 'P': 
					uppercase = true;
					__attribute__((fallthrough));
				case 'p': {
					char tmp[11];
					num_to_ascii(tmp, va_arg(args, uintptr_t),
					16, precision == 0 ? 11 : precision, true, prefix, uppercase);
					for (int i = 0; tmp[i] != '\0'; i++)
						*buf++ = tmp[i];
					break;
				}
				case 'X': 
					uppercase = true;
					__attribute__((fallthrough));
				case 'x': {
					char tmp[11];
					num_to_ascii(tmp, va_arg(args, uint32_t),
					16, precision == 0 ? 11 : precision, true, prefix, uppercase);
					for (int i = 0; tmp[i] != '\0'; i++)
						*buf++ = tmp[i];
					break;
				}
				case 'B': 
					uppercase = true;
					__attribute__((fallthrough));
				case 'b': {
					char tmp[35];
					num_to_ascii(tmp, va_arg(args, uint32_t),
					2, precision == 0 ? 32 : precision, true, prefix, uppercase);
					for (int i = 0; tmp[i] != '\0'; i++)
						*buf++ = tmp[i];
					break;
				}
				
				/* lx = 32 bit, llx = 64 bit */
				case 'l': {
					fmt++;
					if (*fmt == 'x' || *fmt == 'X') {
						char tmp[11];
						num_to_ascii(tmp, va_arg(args, uint32_t),
						16, precision == 0 ? 11 : precision, true, prefix, *fmt == 'x' 
											? false : true);
						for (int i = 0; tmp[i] != '\0'; i++)
							*buf++ = tmp[i];
						break;
					}
					else if (*fmt == 'u') {
						char tmp[21];
						num_to_ascii(tmp, va_arg(args, uint32_t), 10, 0, true, false, false);
						for (int i = 0; tmp[i] != '\0'; i++)
							*buf++ = tmp[i];
						break;
					}
					else if (*fmt == 'l') {
						fmt++;
						/* im so sorry */
						if (*fmt == 'x' || *fmt == 'p' || *fmt == 'X' || *fmt == 'P') {
							/* check if its uppercase, if not uppercase == false*/
							uppercase = *fmt == 'X' ? true : false || *fmt == 'P' ? true : false;
							char tmp[21];
							num_to_ascii(tmp, va_arg(args, uint64_t),
							16, precision == 0 ? 16 : precision, true, prefix, uppercase);
							for (int i = 0; tmp[i] != '\0'; i++)
								*buf++ = tmp[i];
							break;
						}
						else if (*fmt == 'u') {
							char tmp[21];
							num_to_ascii(tmp, va_arg(args, uint64_t), 10, 0, true, false, false);
							for (int i = 0; tmp[i] != '\0'; i++)
								*buf++ = tmp[i];
							break;
						} 
						else if (*fmt == 'b') {
							char tmp[67];
							num_to_ascii(tmp, va_arg(args, uint64_t),
							2, precision == 0 ? 64 : precision, true, prefix, 
								*fmt == 'B' ? true : false);
							for (int i = 0; tmp[i] != '\0'; i++)
								*buf++ = tmp[i];
							break;
						}
					}
					break;
				}
				case '%':
					*buf++ = *fmt;
					break;
				default:
					*buf++ = *fmt;
					break;
			}
		} else {
			*buf++ = *fmt;
		}
		precision = 0;
		prefix = false;
		uppercase = false;
		fmt++;
		count++;
	}
	*buf = '\0';
	return count;
}

int printk(const char *fmt, ...) 
{
	char buf[2048];
	va_list args;
	va_start(args, fmt);
/* use the vsprintf because thats what vsprintf is for */
	int i = vsprintf(buf, fmt, args);
	va_end(args);
	con_puts(&kcon, buf);
	return i;
}

/* returns 0 if successful and -1 if not 
 * outputs to kcon 
 */
int lprintk(int level, const char *fmt, ...)
{
	char buf[2048];
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	switch (level) {
		case KERNEL_LOG_DEBUG:
			con_puts(&kcon, "\033[34m");
			break;
		case KERNEL_LOG_ERROR:
			con_puts(&kcon, "\033[31m");
			break;
		default:
			return -1;
	}
	con_puts(&kcon, buf);
	con_puts(&kcon, "\033[0m");
	return 0;
}
