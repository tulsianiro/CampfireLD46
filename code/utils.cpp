#define global        static
#define internal      static
#define local_persist static

#define bytes(n)       (n)
#define kilobytes(n)   ((n)*1024)
#define megabytes(n)   (kilobytes(n)*1024)
#define gigabytes(n)   (megabytes(n)*1024)
#define terabytes(n)   (gigabytes(n)*1024)

#define array_count(a) (sizeof(a) / sizeof(a[0]))

#define is_odd(a)  ((int)(a) & 0x01)
#define is_even(a) (!is_odd(a))

#define PI  3.1415926535897
#define PIf 3.1415926f

#define deg2rad(a) ((a)*(PI/180.0))
#define rad2deg(a) ((a)*(180.0/PI))
#define deg2radf(a) ((a)*(PIf/180.f))
#define rad2degf(a) ((a)*(180.f/PIf))

#define is_pow2(x) (((x) != 0) && ((x) & ((x)-1)) == 0)
#define align_down(n, a) ((n) & ~((a) - 1))
#define align_up(n, a) ALIGN_DOWN((n) + (a) - 1, (a))
#define align_down_ptr(p, a) ((void *)ALIGN_DOWN((uintptr_t)(p), (a)))
#define align_up_ptr(p, a) ((void *)ALIGN_UP((uintptr_t)(p), (a)))

inline char char_to_lower(char a) { return (a >= 'A' && a <= 'Z') ? (a + 32) : a; }
inline char char_to_upper(char a) { return (a >= 'a' && a <= 'z') ? (a - 32) : a; }

typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8     bool8;
typedef int16    bool16;
typedef int32    bool32;
typedef int64    bool64;
typedef float    real32;
typedef double   real64;
typedef float    float32;
typedef double   float64;

typedef int8     i8;
typedef int16    i16;
typedef int32    i32;
typedef int64    i64;
typedef int8     s8;
typedef int16    s16;
typedef int32    s32;
typedef int64    s64;
typedef int8     b8;
typedef int16    b16;
typedef int32    b32;
typedef int64    b64;
typedef uint8    u8;
typedef uint16   u16;
typedef uint32   u32;
typedef uint64   u64;
typedef real32   r32;
typedef real64   r64;
typedef r32      f32;
typedef r64      f64;

internal b32 file_exists(const char *filename) 
{
    FILE *file;
	fopen_s(&file, filename, "r");
	if(file) 
	{
        fclose(file);
        return 1;
    }
    return 0;
}

internal char *read_file(const char *path, u32 *file_size = 0) 
{
	FILE *file; 
	fopen_s(&file, path, "rb");
    if (!file) 
	{
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long len = ftell(file);
	if (file_size)
	{
		*file_size = len;
	}

    fseek(file, 0, SEEK_SET);
    char *buf = (char *)malloc(len + 1);
    if (len && fread(buf, len, 1, file) != 1) 
	{
        fclose(file);
        free(buf);
        return NULL;
    }
    fclose(file);   
    buf[len] = 0;
    return buf;
}

internal bool write_file(const char *path, const char *buf, size_t len) 
{
	FILE *file;
	fopen_s(&file, path, "w");
    if (!file) 
	{
        return false;
    }
    size_t n = fwrite(buf, len, 1, file);
    fclose(file);
    return n == 1;
}
