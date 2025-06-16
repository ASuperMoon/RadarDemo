#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#pragma pack(push, 1)
// 航迹协议
struct RadarTrack {
  uint head;               // 帧头
  ushort length;           // 整个数据包 报文字节数
  ushort radarID;          // 雷达ID
  quint64 time;            // 1ms UTC时间 时间戳
  ushort xinZaoBi;         // 信噪比
  ushort qiPiFlag;         // 0为起批未确定 起批标志	1为确定目标	2该目标丢弃
  ushort aimID;            // 目标的编号 目标编号
  ushort aimCount;         // 跟踪次数 点迹个数
  ushort aimselfHangXiang; // 目标运动的方向 目标   航向角
  short aimfE;             // 目标与雷达的连线与水平面夹角 目标仰角
  uint aimfR;              // 目标距雷达斜距 目标距离
  short aimSelfVp;         // 0.1m/s 目标运动的速度 目标航速
  short aimVp;             // 0.1m/s 目标相对雷达的径向速度, 远离雷达为正，靠近雷达为负;		目标速度
  short aimfA;             // 目标偏北角  -18000～18000	0.01度 为目标和雷达连线与雷达正北的夹角，目标北偏西时值域为0到-18000度，北偏东时值域为0到18000度
  double B;                // 目标纬度
  double L;                // 目标经度
  float aimHeight;         // 目标海拔高度 目标高度
  uchar sendMode;          // 0为发送所有航迹 发送航迹模式	1为发送单条选中航迹
  uchar selectSata;        // 选择状态
  uchar targetKind;        // 0其它 1车辆 2四旋翼 3固定翼 4鸟 5船 6行人 7客机
  uchar livingChance;      // 目标类别划分可信度	0-100
  ushort aimFuDu;          // 目标幅度
  ushort aimRcs;           // 目标RCS
  ushort aimFangWeiJiao;   // 目标方位角 目标相对于机械0位的偏角 含频率偏角
  uchar dangerLevel;       // 威胁等级
  uint beamNum;            // 波位号
  uint framCnt;            // 帧计数
  uchar selSource;         // 0 自动选中 1手动选中
  ushort noiseAmp;         // 噪声幅度
  quint64 fristTime;       // 首次出现时间
  uchar trackingFlag;
  char resv1[35];
  uint checkSum;           // 累加和 校验和
};

// 状态协议
struct RadarState {
    uint head;                    // 帧头
    ushort lenth;                 // 报文总字节数
    quint64 time;                 // 时间戳

    ushort recReduce:6;           // 接收衰减    6bit
    ushort drivingSwitch:1;       // 激励开关    1bit
    ushort wave:3;                // 雷达波形    3bit
    ushort fft:3;                 // 积累脉冲数  3bit
    ushort mode:3;                // 工作模式     3bit

    ushort jianCethreshold;       // 检测门限
    ushort thresholdFactor;       // 门限因子

    uint rec:27;                  // Resv        27bit
    uint stcSwitch:1;             // STC开关	     1bit
    uint lunFeiSwitch:1;          // 轮飞选择     1bit
    uint followSwitch:1;          // 跟踪开关     1bit
    uint followChoose:1;          // 跟踪选择     1bit
    uint radarConnectSata:1;      // 雷达连接状态 1bit

    float cpiTime;                // CPI时间
    uint holdingTime;             // 周期持续时间
    uint losePackCount;           // 丢包数
    float dataZhuanTaiFuYang;     // 转台俯仰数据
    float tiaoZhiFuYang;          // 调制俯仰数据
    ushort aimIDFollow;           // 跟踪目标编号
    short angleCorrect;           // 角度修正
    ushort zhuanTaiFangWeiAngle;  // 转台方位角
    short zhuanTaiFuYangAngle;    // 转台俯仰角
    ushort luoPanFangWei;         // 罗盘方位角
    short luoPanFuYang;           // 罗盘俯仰
    short luoPanHengQing;         // 罗盘横倾
    double radarJingDu;           // 雷达经度
    double radarWeiDu;            // 雷达纬度
    float radarHeight;            // 雷达高度
    ushort radarKind;             // 雷达类型
    uint radarID;                 // 雷达ID
    uchar clutterMap;
    uchar sateNum;                // 卫星个数

    uchar resv1;                  // 预留
    ushort vMax;                  // 速度上限
    ushort vMin;                  // 速度下限
    ushort hMin;                  // 高度下限
    ushort hMax;                  // 高度上限
    uint dMin;                    // 距离下限
    uint dMax;                    // 距离上限

    ushort startAngle;            // 起始角度
    ushort endAngle;              // 终止角度
    uchar trackingMode;           // 跟踪模式
    uchar searchMode;             // 搜索模式
    uchar ptzAzLockState;         // 方位自锁状态
    ushort arrayCurrent;          // 阵面电流
    uchar arrayCurrentState;      // 电流状态
    uchar digitalState;           // 数处连接状态
    uchar INSLocationState;       // 差分定向状态
    uchar resv;
    uint checksum;                // 校验码
};

// 点迹
struct PointOut{
   ushort fA;  // 目标方位角
   short fE;   // 目标仰角
   int fR;     // 目标距离
   int Vp;     // 目标速度
   int fuDu;   // 目标幅度
};

// 点迹协议
struct RadarPoint {
   uint head;             // 帧头
   ushort length;         // 报文字节数
   ushort radarID;        // 雷达ID
   quint64 time;          // 时间戳
   ushort resv0;
   ushort targetNum;      // 目标个数
   PointOut pointOut[30]; // 目标点信息
   char resv1[8];         // 预留
   int crc;               // 校验码
};

// 过界协议
struct RadarPTZ {
   uint head;                // 帧头
   ushort length;            // 报文字节数
   ushort radarID;           // 雷达ID
   quint64 time;             // 时间戳
   ushort radarAngle;        // 转台机械方位角
   ushort radarPianBeiAngle; // 转台偏北方位角
   char resv[40];            // 预留
   uint crc;                 // 校验码
};

// 控制协议
struct ControlBlock
{
   qint32 funId; //功能号
   qint32 resv0; // 预留
   qint32 resv1; // 预留
   qint32 data0; // 数据0
   qint32 data1; // 数据1
   qint32 data2; // 数据2
   qint32 data3; // 数据3
   double data4; // 数据4
   double data5; // 数据5
   double data6; // 数据6
   double data7; // 数据7
};
struct RadarHeader {
    quint32 head = 0x56482785;
    quint32 length = 0;
    char resv0[56];
    quint32 blockCount = 0;
};
struct RadarControl {
    RadarHeader header;
    std::vector<ControlBlock> blocks;

    RadarControl(quint32 count = 0){
       header.blockCount = count;
       header.head = 0x56482785;
       blocks.resize(count);
       header.length = sizeof(RadarControl) - sizeof(std::vector<ControlBlock>) + count * sizeof(ControlBlock);
    }

    QByteArray serialize() const {
        QByteArray data;
        data.append(reinterpret_cast<const char*>(&header), sizeof(header));
        data.append(reinterpret_cast<const char*>(blocks.data()),
                    blocks.size() * sizeof(ControlBlock));
        return data;
    }
};

// UI结构体
struct UIConfig {
  float startScan0;
  float startScan1;
  float startScan2;
  float startScan3;
  float endScan0;
  float endScan1;
  float endScan2;
  float endScan3;
  float northCorrect;
  float pitchCorrect;
  float distanceCorrect;
  double radarLng;         // 经度
  double radarLon;         // 纬度
  float radarHeight;
  float speedMin;
  float speedMax;
  float heightMin;
  float heightMax;
  float distanceMin;
  float distanceMax;
};

#pragma pack(pop)

class UDPClass;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void controlInit();
    void configUpdate();
    void configUIUpdate();
    void configSave();
    void dataProcess(QByteArray data);
    void on_PowerOnApplyPushButton_clicked();
    void on_PowerOffApplyPushButton_clicked();
    void on_ApplyPushButton_clicked();
private:
    Ui::MainWindow *ui;
    UDPClass *radarUDP;
    RadarTrack track;
    RadarState state;
    RadarPoint point;
    RadarPTZ ptz;
    UIConfig config;

    QString localIP;
    qint32 localPort;
    QString remoteIP;
    qint32 remotePort;
};
#endif // MAINWINDOW_H
