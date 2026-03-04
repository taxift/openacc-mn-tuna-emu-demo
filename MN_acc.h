// ホストコード側の関数宣言（このファイルのinclude行はコンパイル時に自動で挟まれる）
// 元々XcodeML-toolsのバックエンドの実装として、_ACCなどで始まる関数については、
// XmcXcodeToXcTranslator.javaの2677行目付近でnot definedにならないように弾いている
// MN-Core実装でもそれを踏襲して、現状のXcodeMLバックエンドでも通るようにしている

void _ACC_MN_runtime_HtoD(void* var, int end);
void _ACC_MN_runtime_DtoH(void* var, int end);
void _ACC_MN_runtime_kernel_kick(char* kernel_name);
