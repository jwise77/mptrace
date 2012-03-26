#ifdef r4
#define FLOAT float
#define FSYM "f"
#define GOUTSYM ".7g"
#define ROUNDOFF 1e-6
typedef int long_int;
#endif /* r4 */

#ifdef r8
#define FLOAT double
#define FSYM "lf"
#define GOUTSYM ".14g"
#define ROUNDOFF 1e-12
typedef long long long_int;
#endif /* r8 */

#ifdef r16

#ifdef FSYM_lf
#define FSYM "lf"
#define GOUTSYM ".21G"
#else
#define FSYM "Lf"
#define GOUTSYM ".21Lg"
#endif

#define ROUNDOFF 1e-16
#define FLOAT long_double
typedef long long long_int;
#endif /* r16 */

typedef long double long_double;

