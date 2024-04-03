/*
所有函数返回值均为int, -1表示发生异常
当异常发生时，可通过 get_last_error 方法获取最近一次异常文本内容，pErr由调用方申请内存，不能小于256字节
*/

/*
获取最近一次异常的文本信息
*/
int get_last_error(char* pErr);

/*
扫描板卡，固定耗时500ms, 返回可用板卡数
*/
int scan_modules();

/*
查新板卡信息
index 板卡序号0 ~ n-1
pModuleInfo 板卡信息json字符串，调用方申请足够空间,例:{"deviceId":"xxxxx","productName":"BRC9234","channelCount":4}
*/
int get_module_info(int index, char* pModuleInfo);

/*
连接板卡, 对板卡的所有操作均要在连接成功之后进行
index 板卡序号0 ~ n-1
返回值mHandle 用于操作已建立连接的板卡
*/
int connect_module(int index);

/*
断开板卡
*/
int disconnect_module(int mHandle);

/*
配置板卡参数
sampleRate 采样率, 板卡会自动适配最近的可用采样率, 如需知道实际采样率，请调用查询函数
couplingMode 耦合方式: 0 -> DC , 1 -> AC
current 激励电流(mA)
*/
int set_parameters(int mHandle, double sampleRate, double gain, int couplingMode, double current);

/*
查询板卡参数实际参数
sampleRate 采样率
couplingMode 耦合方式: 0 -> DC , 1 -> AC
current 激励电流(mA)
*/
int get_parameters(int mHandle, double* pSampleRate, double* pGain, int* pCouplingMode, double* pCurrent);

/*
开始采样
rawValue 是否取ADC原始值, false:正常使用,后续返回电压值;  true:定标校准时使用, 后续返回ADC原始值
*/
int start(int mHandle, bool rawValue);

/*
停止采样, 会清空内存中还未提取的数据
*/
int stop(int mHandle);

/*
获取所有通道数据
data_array 数据指针, 调用方申请空间, sdk填充数据
length 希望获取的点数, 请注意, 该函数将在指针data_array指向的内存填充 N x length 长度的数据,N 为板卡通道数
数据以交织方式填充 例如: 1111 2222 3333 4444
data_array_length 数组总长度 N x length ,labview需要传入这个参数，但SDK用不到
timeout 超时时间(ms), 指定时间内若仍未有足够的点数将触发超时错误
*/
int get_channels_data(int mHandle, double* data_array, int length, int timeout, int data_array_length);

/*
设置指定通道在指定增益下的校准参数, 此函数调用后会立即生效, 但不会持久化, 重启后将复原
若要固化参数，请调用 save_calibration 方法
channel 通道号 0 - N-1
gain 增益, 注意：gain填错将不会生效
K 系数 K
B 常数 B,  注：Y=KX+B, X为ADC原始值, Y为电压值
*/
int set_calibration(int mHandle, int channel, double gain, double K, double B);

/*
将当前的校准参数全部保存至flash 持久化
*/
int save_calibration(int mHandle);

/*
清除内存和flash中的所有校准参数, 复位成出厂设置
*/
int reset_calibration(int mHandle);

/*
获取模块温度
pChipTemperature 芯片温度（指针）
pAmbientTemperature 环境温度（指针）
*/
int check_temperature(int mHandle, float* pChipTemperature, float* pAmbientTemperature);