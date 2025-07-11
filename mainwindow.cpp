#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Net/udpclass.h"
#include <QSettings>
#include <QDateTime>
#include <QtMath>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    setWindowTitle("Radar Simulator");
    controlInit();
    radarUDP = new UDPClass();
    radarUDP->Bind(localIP,localPort);
    connect(radarUDP,&UDPClass::DataReceived,this,&MainWindow::dataProcess);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 结构体初始化
void MainWindow::controlInit()
{
   QSettings * settings = new QSettings( QDir::cleanPath(QCoreApplication::applicationDirPath() + QDir::separator() + QString("RadarConfig.ini")),QSettings::IniFormat);
   // 界面参数
   config.startScan0 = (float)settings->value("UIStruct/startScan0",0).toDouble();
   config.startScan1 = (float)settings->value("UIStruct/startScan1",0).toDouble();
   config.startScan2 = (float)settings->value("UIStruct/startScan2",0).toDouble();
   config.startScan3 = (float)settings->value("UIStruct/startScan3",0).toDouble();
   config.endScan0 = (float)settings->value("UIStruct/endScan0",360).toDouble();
   config.endScan1 = (float)settings->value("UIStruct/endScan1",0).toDouble();
   config.endScan2 = (float)settings->value("UIStruct/endScan2",0).toDouble();
   config.endScan3 = (float)settings->value("UIStruct/endScan3",0).toDouble();
   config.northCorrect = (float)settings->value("UIStruct/northCorrect",0).toDouble();
   config.pitchCorrect = (float)settings->value("UIStruct/pitchCorrect",0).toDouble();
   config.distanceCorrect = (float)settings->value("UIStruct/distanceCorrect",0).toDouble();
   config.radarLng = settings->value("UIStruct/radarLng",117.214480).toDouble();
   config.radarLon = settings->value("UIStruct/radarLon",31.714677).toDouble();
   config.radarHeight = (float)settings->value("UIStruct/radarHeight",0).toDouble();
   config.speedMin = (float)settings->value("UIStruct/speedMin",0.5).toDouble();
   config.speedMax = (float)settings->value("UIStruct/speedMax",50).toDouble();
   config.heightMin = (float)settings->value("UIStruct/heightMin",0).toDouble();
   config.heightMax = (float)settings->value("UIStruct/heightMax",1000).toDouble();
   config.distanceMin = (float)settings->value("UIStruct/distanceMin",0).toDouble();
   config.distanceMax = (float)settings->value("UIStruct/distanceMax",100000).toDouble();

   localIP = settings->value("UDP/localIP","127.0.0.1").toString();
   localPort = settings->value("UDP/localPort",20001).toInt();
   remoteIP = settings->value("UDP/remoteIP","127.0.0.1").toString();
   remotePort = settings->value("UDP/remotePort",20000).toInt();
   settings->setValue("UDP/localIP", localIP);
   settings->setValue("UDP/localPort", localPort);
   settings->setValue("UDP/remoteIP", remoteIP);
   settings->setValue("UDP/remotePort", remotePort);

   delete settings;
   settings = nullptr;
   configUIUpdate();
   configSave();
}

// 结构体初更新
void MainWindow::configUpdate()
{
    config.startScan0 = ui->StartSacn0LineEdit->text().toFloat();
    config.startScan1 = ui->StartSacn1LineEdit->text().toFloat();
    config.startScan2 = ui->StartSacn2LineEdit->text().toFloat();
    config.startScan3 = ui->StartSacn3LineEdit->text().toFloat();
    config.endScan0 = ui->EndSacn0LineEdit->text().toFloat();
    config.endScan1 = ui->EndSacn1LineEdit->text().toFloat();
    config.endScan2 = ui->EndSacn2LineEdit->text().toFloat();
    config.endScan3 = ui->EndSacn3LineEdit->text().toFloat();
    config.northCorrect = ui->NorthCorrectionLineEdit->text().toFloat();
    config.pitchCorrect = ui->ECorrectionLineEdit->text().toFloat();
    config.distanceCorrect = ui->RCorrectionLineEdit->text().toFloat();
    config.radarLng = ui->RadarLngLineEdit->text().toDouble();
    config.radarLon = ui->RadarLonLineEdit->text().toDouble();
    config.radarHeight = ui->RadarHightLineEdit->text().toFloat();
    config.speedMin = ui->SpeedMinLineEdit->text().toFloat();
    config.speedMax = ui->SpeedMaxLineEdit->text().toFloat();
    config.heightMin = ui->HeightMinLineEdit->text().toFloat();
    config.heightMax = ui->HeightMaxLineEdit->text().toFloat();
    config.distanceMin = ui->DistanceMinLineEdit->text().toFloat();
    config.distanceMax = ui->DistanceMaxLineEdit->text().toFloat();
}

// UI更新
void MainWindow::configUIUpdate()
{
   ui->StartSacn0LineEdit->setText(QString::number(config.startScan0, 'f', 2));
   ui->StartSacn1LineEdit->setText(QString::number(config.startScan1, 'f', 2));
   ui->StartSacn2LineEdit->setText(QString::number(config.startScan2, 'f', 2));
   ui->StartSacn3LineEdit->setText(QString::number(config.startScan3, 'f', 2));

   ui->EndSacn0LineEdit->setText(QString::number(config.endScan0, 'f', 2));
   ui->EndSacn1LineEdit->setText(QString::number(config.endScan1, 'f', 2));
   ui->EndSacn2LineEdit->setText(QString::number(config.endScan2, 'f', 2));
   ui->EndSacn3LineEdit->setText(QString::number(config.endScan3, 'f', 2));

   ui->NorthCorrectionLineEdit->setText(QString::number(config.northCorrect, 'f', 2));
   ui->ECorrectionLineEdit->setText(QString::number(config.pitchCorrect, 'f', 2));
   ui->RCorrectionLineEdit->setText(QString::number(config.distanceCorrect));

   ui->RadarLngLineEdit->setText(QString::number(config.radarLng, 'f', 6));
   ui->RadarLonLineEdit->setText(QString::number(config.radarLon, 'f', 6));
   ui->RadarHightLineEdit->setText(QString::number(config.radarHeight));

   ui->SpeedMinLineEdit->setText(QString::number(config.speedMin, 'f', 1));
   ui->SpeedMaxLineEdit->setText(QString::number(config.speedMax, 'f', 1));
   ui->HeightMinLineEdit->setText(QString::number(config.heightMin));
   ui->HeightMaxLineEdit->setText(QString::number(config.heightMax));
   ui->DistanceMinLineEdit->setText(QString::number(config.distanceMin));
   ui->DistanceMaxLineEdit->setText(QString::number(config.distanceMax));
}

// 结构体保存
void MainWindow::configSave()
{
   QMetaObject::invokeMethod(this, [=]() {
    QSettings * settings = new QSettings( QDir::cleanPath(QCoreApplication::applicationDirPath() + QDir::separator() + QString("RadarConfig.ini")),QSettings::IniFormat);
    settings->setValue("UIStruct/startScan0", QString::number(config.startScan0));
    settings->setValue("UIStruct/startScan1", QString::number(config.startScan1));
    settings->setValue("UIStruct/startScan2", QString::number(config.startScan2));
    settings->setValue("UIStruct/startScan3", QString::number(config.startScan3));

    settings->setValue("UIStruct/endScan0", QString::number(config.endScan0));
    settings->setValue("UIStruct/endScan1", QString::number(config.endScan1));
    settings->setValue("UIStruct/endScan2", QString::number(config.endScan2));
    settings->setValue("UIStruct/endScan3", QString::number(config.endScan3));

    settings->setValue("UIStruct/northCorrect", QString::number(config.northCorrect));
    settings->setValue("UIStruct/pitchCorrect", QString::number(config.pitchCorrect));
    settings->setValue("UIStruct/distanceCorrect", QString::number(config.distanceCorrect));

    settings->setValue("UIStruct/radarLng", QString::number(config.radarLng));
    settings->setValue("UIStruct/radarLon", QString::number(config.radarLon));
    settings->setValue("UIStruct/radarHeight", QString::number(config.radarHeight));

    settings->setValue("UIStruct/speedMin", QString::number(config.speedMin));
    settings->setValue("UIStruct/speedMax", QString::number(config.speedMax));

    settings->setValue("UIStruct/heightMin", QString::number(config.heightMin));
    settings->setValue("UIStruct/heightMax", QString::number(config.heightMax));

    settings->setValue("UIStruct/distanceMin", QString::number(config.distanceMin));
    settings->setValue("UIStruct/distanceMax", QString::number(config.distanceMax));
   }, Qt::QueuedConnection);
}

// 协议解析
void MainWindow::dataProcess(QByteArray data)
{
     if(data.length() < 4){
         return;
     }
     uint head;
     memcpy(&head, data.data(), sizeof(head));
     // 航迹协议
     if(head == 0XCC5555CC){
        memcpy(&track, data.data(), sizeof(track));
        QString trackStr = "";
        quint64 timeDif = QDateTime::currentDateTimeUtc().toMSecsSinceEpoch();
        trackStr += QString("时间差: %1%2\n").arg( qAbs(track.time - timeDif)).arg("ms");
        trackStr += QString("时间戳: %1%2\n").arg(track.time).arg("ms");
        trackStr += QString("信噪比: %1%2\n").arg(track.xinZaoBi).arg("dB");
        trackStr += QString("起批标志: %1\n").arg(track.qiPiFlag);
        trackStr += QString("目标的编号: %1\n").arg(track.aimID);
        trackStr += QString("跟踪次数: %1\n").arg(track.aimCount);
        trackStr += QString("航向角: %1%2\n").arg(track.aimselfHangXiang / 100.0f ,0, 'f', 2).arg("°");
        trackStr += QString("俯仰角: %1%2\n").arg(track.aimfE / 100.0f ,0, 'f', 2).arg("°");
        trackStr += QString("距离: %1%2\n").arg(track.aimfR).arg("m");
        trackStr += QString("目标航速: %1%2\n").arg(track.aimSelfVp / 10.0f ,0, 'f', 2).arg("m/s");
        trackStr += QString("目标速度: %1%2\n").arg(track.aimVp / 10.0f ,0, 'f', 2).arg("m/s");
        trackStr += QString("目标偏北角: %1%2\n").arg(track.aimfA / 100.0f ,0, 'f', 2).arg("°");
        trackStr += QString("目标纬度: %1%2\n").arg(track.B ,0, 'f', 6).arg("°");
        trackStr += QString("目标经度: %1%2\n").arg(track.L ,0, 'f', 6).arg("°");
        trackStr += QString("目标高度: %1%2\n").arg(track.aimHeight ,0, 'f', 2).arg("m");
        trackStr += QString("发送模式: %1\n").arg(track.sendMode);
        trackStr += QString("选择状态: %1\n").arg(track.selectSata);
        trackStr += QString("类型: %1\n").arg(track.targetKind);
        trackStr += QString("可信度: %1%2\n").arg(track.livingChance).arg("%");
        trackStr += QString("目标幅度: %1%2\n").arg(track.aimFuDu).arg("dBm");
        trackStr += QString("目标RCS: %1%2\n").arg(track.aimRcs / 1000.0f ,0, 'f',4).arg("m^2");
        trackStr += QString("目标方位角: %1%2\n").arg(track.aimFangWeiJiao / 100.0f ,0, 'f', 6).arg("°");
        trackStr += QString("威胁等级: %1\n").arg(track.dangerLevel);
        trackStr += QString("波位号: %1\n").arg(track.beamNum);
        trackStr += QString("帧计数: %1\n").arg(track.framCnt);
        trackStr += QString("噪声幅度: %1%2\n").arg(track.noiseAmp).arg("dBm");
        ui->RadarTrackTextEdit->setText(trackStr);
     }
     // 状态协议
     else if(head == 0x55DDDD55){
        memcpy(&state, data.data(), sizeof(state));
        QString stateStr = "";
        stateStr += QString("时间戳: %1%2\n").arg(state.time).arg("ms");
        stateStr += QString("工作模式: %1\n").arg(state.mode);
        stateStr += QString("积累点数: %1\n").arg(state.fft);
        stateStr += QString("雷达波形: %1\n").arg(state.wave);
        stateStr += QString("激励开关: %1\n").arg(state.drivingSwitch);
        stateStr += QString("接收衰减: %1%2\n").arg(state.recReduce).arg("dB");
        stateStr += QString("检测门限: %1%2\n").arg(state.jianCethreshold).arg("dBm");
        stateStr += QString("门限因子: %1%2\n").arg(state.thresholdFactor).arg("dB");
        stateStr += QString("雷达连接状态: %1\n").arg(state.radarConnectSata);
        stateStr += QString("丢包数: %1\n").arg(state.losePackCount);
        stateStr += QString("角度修正: %1%2\n").arg(state.angleCorrect / 100.0f ,0, 'f', 2).arg("°");
        stateStr += QString("转台方位角: %1%2\n").arg(state.zhuanTaiFangWeiAngle / 100.0f ,0, 'f', 2).arg("°");
        stateStr += QString("雷达经度: %1%2\n").arg(state.radarJingDu,0, 'f', 6).arg("°");
        stateStr += QString("雷达纬度: %1%2\n").arg(state.radarWeiDu,0, 'f', 6).arg("°");
        stateStr += QString("雷达高度: %1%2\n").arg(state.radarHeight,0, 'f', 2).arg("m");
        stateStr += QString("速度上限: %1%2\n").arg(state.vMax / 10.0f , 0, 'f', 2).arg("m/s");
        stateStr += QString("速度下限: %1%2\n").arg(state.vMin / 10.0f ,0, 'f', 2).arg("m/s");
        stateStr += QString("高度下限: %1%2\n").arg(state.hMin).arg("m");
        stateStr += QString("高度上限: %1%2\n").arg(state.hMax).arg("m");
        stateStr += QString("距离下限: %1%2\n").arg(state.dMin).arg("m");
        stateStr += QString("距离上限: %1%2\n").arg(state.dMax).arg("m");
        stateStr += QString("起始角度: %1%2\n").arg(state.startAngle / 100.0f ,0, 'f',2).arg("°");
        stateStr += QString("终止角度: %1%2\n").arg(state.endAngle / 100.0f ,0, 'f', 2).arg("°");
        stateStr += QString("阵面电流: %1%2\n").arg(state.arrayCurrent / 10.0f,0, 'f',1).arg("A");
        stateStr += QString("电流状态: %1\n").arg(state.arrayCurrentState);
        ui->RadarStateTextEdit->setText(stateStr);
     }
     // 过界协议
     else if(head == 0x55CCCC55){
        memcpy(&ptz, data.data(), sizeof(ptz));
        QString ptzStr = "";
        ptzStr += QString("时间戳: %1%2\n").arg(ptz.time).arg("");
        ptzStr += QString("转台机械方位角: %1%2\n").arg(ptz.radarAngle / 100.0f ,0, 'f', 2).arg("°");
        ptzStr += QString("转台偏北方位角: %1%2\n").arg(ptz.radarPianBeiAngle / 100.0f ,0, 'f', 2).arg("°");
        ui->RadarPTZTextEdit->setText(ptzStr);
     }
     // 点迹协议
     else if(head == 0xAA5555AA){
        memcpy(&point, data.data(), sizeof(point));
        QString pointStr = "";
        pointStr += QString("时间戳: %1%2\n").arg(point.time).arg("ms");
        pointStr += QString("目标个数: %1\n").arg(point.targetNum);
        pointStr += QString("点迹信息：\n");
        // 循环输出目标点信息
        for (int i = 0; i < point.targetNum; ++i) {
            pointStr += QString(" 方位角: %1%2").arg(point.pointOut[i].fA / 100.0f ,0, 'f', 2).arg("°");
            pointStr += QString("仰角: %1%2").arg(point.pointOut[i].fE / 100.0f ,0, 'f', 2).arg("°");
            pointStr += QString("距离: %1%2").arg(point.pointOut[i].fR).arg("m");
            pointStr += QString("速度: %1%2").arg(point.pointOut[i].Vp / 10.0f ,0, 'f', 2).arg("m/s");
            pointStr += QString("幅度: %1%2\n").arg(point.pointOut[i].fuDu).arg("dBm");
        }
        ui->RadarPointTextEdit->setText(pointStr);
     }
     // 信处参数
     else if(head == 0x55AAAA55){
        memcpy(&spConfig, data.data(), sizeof(spConfig));
        QString SPStr = "";
        SPStr += QString("帧头 uint head: %1\n").arg(spConfig.head);
        SPStr += QString("字节数 uint length: %1\n").arg(spConfig.length);
        SPStr += QString("发射起始扇区 float emiteStartAngle0: %1\n").arg(spConfig.emiteStartAngle0);
        SPStr += QString("发射终止扇区 float emiteEndAngle0: %1\n").arg(spConfig.emiteEndAngle0);
        SPStr += QString("激励发射开关 int emiteswitch: %1\n").arg(spConfig.emiteswitch);
        SPStr += QString("搜索模式 int scanMode: %1\n").arg(spConfig.scanMode);
        SPStr += QString("频点 int freNum: %1\n").arg(spConfig.freNum);
        SPStr += QString("信噪比门限 int snrTh: %1\n").arg(spConfig.snrTh);
        SPStr += QString("检测门限 int ampTh: %1\n").arg(spConfig.ampTh);
        SPStr += QString("杂波抑制带宽 int csBandwidth: %1\n").arg(spConfig.csBandwidth);
        SPStr += QString("转台轴 int ptzAxi: %1\n").arg(spConfig.ptzAxi);
        SPStr += QString("转台运动模式 int ptzMode: %1\n").arg(spConfig.ptzMode);
        SPStr += QString("转台定位角度 float ptzLocateAngle: %1\n").arg(spConfig.ptzLocateAngle);
        SPStr += QString("转台扇扫中心 float ptzSectCent: %1\n").arg(spConfig.ptzSectCent);
        SPStr += QString("转台扇扫单侧角度 float ptzSectSize: %1\n").arg(spConfig.ptzSectSize);
        SPStr += QString("转台速度 float ptzSpeed: %1\n").arg(spConfig.ptzSpeed);
        SPStr += QString("stc使能 int stc_en: %1\n").arg(spConfig.stc_en);
        SPStr += QString("stc选择 int stc_sel: %1\n").arg(spConfig.stc_sel);
        SPStr += QString("时序模式 int frame_acc_en: %1\n").arg(spConfig.frame_acc_en);
        SPStr += QString("校验位 int checkSum: %1\n").arg(spConfig.checkSum);
        qDebug()<<SPStr;
     }
     // 数处参数
     else if(head == 0x55222255){
         memcpy(&dpconfig, data.data(), sizeof(dpconfig));
         QString DPStr = "";
         DPStr += QString("帧头 uint head: %1\n").arg(dpconfig.head);
         DPStr += QString("字节数 uint length: %1\n").arg(dpconfig.length);
         DPStr += QString("雷达类型 uchar radarKind: %1\n").arg(dpconfig.radarKind);
         DPStr += QString("保留 uchar resv0: %1\n").arg(dpconfig.resv0);
         DPStr += QString("雷达ID uint radarID: %1\n").arg(dpconfig.radarID);
         DPStr += QString("保留 uint resv1: %1\n").arg(dpconfig.resv1);
         DPStr += QString("数处处理版本 uint digitalEdition: %1\n").arg(dpconfig.digitalEdition);
         DPStr += QString("显控版本 uint terminalEdition: %1\n").arg(dpconfig.terminalEdition);
         DPStr += QString("调波束跟踪 ushort beamTracking: %1\n").arg(dpconfig.beamTracking);
         DPStr += QString("套波门选择 ushort waveGateTracking: %1\n").arg(dpconfig.waveGateTracking);
         DPStr += QString("扇形跟踪选择  ushort sectorTracking: %1\n").arg(dpconfig.sectorTracking);
         DPStr += QString("指定跟踪航迹号 ushort followAimNum: %1\n").arg(dpconfig.followAimNum);
         DPStr += QString("搜索跟踪选择 ushort scanOrfollowSwitch: %1\n").arg(dpconfig.scanOrfollowSwitch);
         DPStr += QString("航迹起始选择 ushort traceStartConfig: %1\n").arg(dpconfig.traceStartConfig);
         DPStr += QString("杂波图开关 uint pointMapEnable: %1\n").arg(dpconfig.pointMapEnable);
         DPStr += QString("起始滑窗的分母 ushort traceStartN: %1\n").arg(dpconfig.traceStartN);
         DPStr += QString("起始滑窗的分子 ushort traceStartM: %1\n").arg(dpconfig.traceStartM);
         DPStr += QString("滤波模型 ushort filteringModel: %1\n").arg(dpconfig.filteringModel);
         DPStr += QString("关联准则 ushort relationRule: %1\n").arg(dpconfig.relationRule);
         DPStr += QString("消亡周期 int dieCycle: %1\n").arg(dpconfig.dieCycle);

         DPStr += QString("处理距离上限 float rMax: %1\n").arg(dpconfig.rMax);
         DPStr += QString("处理距离下限 float rMin: %1\n").arg(dpconfig.rMin);
         DPStr += QString("处理速度上限 float vMax: %1\n").arg(dpconfig.vMax);
         DPStr += QString("处理速度下限 float vMin: %1\n").arg(dpconfig.vMin);
         DPStr += QString("处理高度上限 float hMax: %1\n").arg(dpconfig.hMax);
         DPStr += QString("处理高度下限 float hMin: %1\n").arg(dpconfig.hMin);

         DPStr += QString("仰角修正 short eCorrection: %1\n").arg(dpconfig.eCorrection);
         DPStr += QString("距离修正 short rCorrection: %1\n").arg(dpconfig.rCorrection);
         DPStr += QString("手动输入仰角值 short antennaElevation: %1\n").arg(dpconfig.antennaElevation);
         DPStr += QString("偏北角修正 short pianBeiXiuZheng: %1\n").arg(dpconfig.pianBeiXiuZheng);
         DPStr += QString("俯仰角选择 int antennaElevationSource: %1\n").arg(dpconfig.antennaElevationSource);
         DPStr += QString("雷达经度 double radarLng: %1\n").arg(dpconfig.radarLng);
         DPStr += QString("雷达纬度 double radarLat: %1\n").arg(dpconfig.radarLat);
         DPStr += QString("雷达高度 float radarH: %1\n").arg(dpconfig.radarH);
         DPStr += QString("是否插值 int interpolationEnable: %1\n").arg(dpconfig.interpolationEnable);
         DPStr += QString("副瓣匿影 int sidelobeBlanking: %1\n").arg(dpconfig.sidelobeBlanking);
         DPStr += QString("杂波图门限 float pointMapThreshold: %1\n").arg(dpconfig.pointMapThreshold);
         DPStr += QString("数据开关 int dataSwitch: %1\n").arg(dpconfig.dataSwitch);
         DPStr += QString("罗盘相关参数设置 int saveCompassData: %1\n").arg(dpconfig.saveCompassData);
         DPStr += QString("数处界面显示开关 int digitalProShowSwitch: %1\n").arg(dpconfig.digitalProShowSwitch);
         DPStr += QString("保留 int resv2: %1\n").arg(dpconfig.resv2);
         DPStr += QString("校验位 int checkSum: %1\n").arg(dpconfig.checkSum);
         qDebug()<<DPStr;
     }
}

// 控制三  开机点击事件
void MainWindow::on_PowerOnApplyPushButton_clicked()
{
   RadarControl control(1);
   control.blocks[0].funId = 1; // 功能号
   control.blocks[0].data0 = 1; // 数据0
   control.blocks[0].data1 = 0; // 数据1
   control.blocks[0].data2 = 0; // 数据2
   control.blocks[0].data3 = 0; // 数据3
   control.blocks[0].data4 = 0; // 数据4
   control.blocks[0].data5 = 0; // 数据5
   control.blocks[0].data6 = 0; // 数据6
   control.blocks[0].data7 = 0; // 数据7
   QByteArray temData = control.serialize();
   radarUDP->UDPSend(temData,remoteIP,remotePort);
   ui->LoggerTextEdit->append(QString("[INFO] %1 Control Power On! \r\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
}

// 控制三  关机点击事件
void MainWindow::on_PowerOffApplyPushButton_clicked()
{
   RadarControl control(1);
   control.blocks[0].funId = 1; // 功能号
   control.blocks[0].data0 = 0; // 数据0
   control.blocks[0].data1 = 0; // 数据1
   control.blocks[0].data2 = 0; // 数据2
   control.blocks[0].data3 = 0; // 数据3
   control.blocks[0].data4 = 0; // 数据4
   control.blocks[0].data5 = 0; // 数据5
   control.blocks[0].data6 = 0; // 数据6
   control.blocks[0].data7 = 0; // 数据7
   QByteArray temData = control.serialize();
   radarUDP->UDPSend(temData,remoteIP,remotePort);
   ui->LoggerTextEdit->append(QString("[INFO] %1 Control Power Off! \r\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
}

// 控制三  控制协议控制
void MainWindow::on_ApplyPushButton_clicked()
{
   configUpdate();
   configSave();
   RadarControl control(11);
   // 扇区0控制 功能号3
   control.blocks[0].funId = 3; // 功能号
   control.blocks[0].data0 = 0; // 数据0
   control.blocks[0].data1 = 0; // 数据1
   control.blocks[0].data2 = 0; // 数据2
   control.blocks[0].data3 = 0; // 数据3
   control.blocks[0].data4 = config.startScan0; // 数据4
   control.blocks[0].data5 = config.endScan0; // 数据5
   control.blocks[0].data6 = 0; // 数据6
   control.blocks[0].data7 = 0; // 数据7
   // 扇区1控制 功能号44
   control.blocks[1].funId = 44; // 功能号
   control.blocks[1].data0 = 0; // 数据0
   control.blocks[1].data1 = 0; // 数据1
   control.blocks[1].data2 = 0; // 数据2
   control.blocks[1].data3 = 0; // 数据3
   control.blocks[1].data4 = config.startScan1; // 数据4
   control.blocks[1].data5 = config.endScan1; // 数据5
   control.blocks[1].data6 = 0; // 数据6
   control.blocks[1].data7 = 0; // 数据7
   // 扇区2控制 功能号45
   control.blocks[2].funId = 45; // 功能号
   control.blocks[2].data0 = 0; // 数据0
   control.blocks[2].data1 = 0; // 数据1
   control.blocks[2].data2 = 0; // 数据2
   control.blocks[2].data3 = 0; // 数据3
   control.blocks[2].data4 = config.startScan2; // 数据4
   control.blocks[2].data5 = config.endScan2; // 数据5
   control.blocks[2].data6 = 0; // 数据6
   control.blocks[2].data7 = 0; // 数据7
   // 扇区3控制 功能号46
   control.blocks[3].funId = 46; // 功能号
   control.blocks[3].data0 = 0; // 数据0
   control.blocks[3].data1 = 0; // 数据1
   control.blocks[3].data2 = 0; // 数据2
   control.blocks[3].data3 = 0; // 数据3
   control.blocks[3].data4 = config.startScan3; // 数据4
   control.blocks[3].data5 = config.endScan3; // 数据5
   control.blocks[3].data6 = 0; // 数据6
   control.blocks[3].data7 = 0; // 数据7
   // 偏北修正 功能号8
   control.blocks[4].funId = 8; // 功能号
   control.blocks[4].data0 = 0; // 数据0
   control.blocks[4].data1 = 0; // 数据1
   control.blocks[4].data2 = 0; // 数据2
   control.blocks[4].data3 = 0; // 数据3
   control.blocks[4].data4 = config.northCorrect; // 数据4
   control.blocks[4].data5 = 0; // 数据5
   control.blocks[4].data6 = 0; // 数据6
   control.blocks[4].data7 = 0; // 数据7
   // 俯仰修正 功能号9
   control.blocks[5].funId = 9; // 功能号
   control.blocks[5].data0 = 0; // 数据0
   control.blocks[5].data1 = 0; // 数据1
   control.blocks[5].data2 = 0; // 数据2
   control.blocks[5].data3 = 0; // 数据3
   control.blocks[5].data4 = config.pitchCorrect; // 数据4
   control.blocks[5].data5 = 0; // 数据5
   control.blocks[5].data6 = 0; // 数据6
   control.blocks[5].data7 = 0; // 数据7
   // 斜距修正 功能号10
   control.blocks[6].funId = 10; // 功能号
   control.blocks[6].data0 = 0; // 数据0
   control.blocks[6].data1 = 0; // 数据1
   control.blocks[6].data2 = 0; // 数据2
   control.blocks[6].data3 = 0; // 数据3
   control.blocks[6].data4 = config.distanceCorrect; // 数据4
   control.blocks[6].data5 = 0; // 数据5
   control.blocks[6].data6 = 0; // 数据6
   control.blocks[6].data7 = 0; // 数据7
   // 雷达经纬高 功能号11
   control.blocks[7].funId = 11; // 功能号
   control.blocks[7].data0 = 0; // 数据0
   control.blocks[7].data1 = 0; // 数据1
   control.blocks[7].data2 = 0; // 数据2
   control.blocks[7].data3 = 0; // 数据3
   control.blocks[7].data4 = config.radarLng; // 数据4
   control.blocks[7].data5 = config.radarLon; // 数据5
   control.blocks[7].data6 = config.radarHeight; // 数据6
   control.blocks[7].data7 = 0; // 数据7
   // 处理速度范围 功能号21
   control.blocks[8].funId = 21; // 功能号
   control.blocks[8].data0 = 0; // 数据0
   control.blocks[8].data1 = 0; // 数据1
   control.blocks[8].data2 = 0; // 数据2
   control.blocks[8].data3 = 0; // 数据3
   control.blocks[8].data4 = config.speedMin; // 数据4
   control.blocks[8].data5 = config.speedMax; // 数据5
   control.blocks[8].data6 = 0; // 数据6
   control.blocks[8].data7 = 0; // 数据7
   // 处理高度范围 功能号22
   control.blocks[9].funId = 22; // 功能号
   control.blocks[9].data0 = 0; // 数据0
   control.blocks[9].data1 = 0; // 数据1
   control.blocks[9].data2 = 0; // 数据2
   control.blocks[9].data3 = 0; // 数据3
   control.blocks[9].data4 = config.heightMin; // 数据4
   control.blocks[9].data5 = config.heightMax; // 数据5
   control.blocks[9].data6 = 0; // 数据6
   control.blocks[9].data7 = 0; // 数据7
   // 处理距离范围 功能号23
   control.blocks[10].funId = 23; // 功能号
   control.blocks[10].data0 = 0; // 数据0
   control.blocks[10].data1 = 0; // 数据1
   control.blocks[10].data2 = 0; // 数据2
   control.blocks[10].data3 = 0; // 数据3
   control.blocks[10].data4 = config.distanceMin; // 数据4
   control.blocks[10].data5 = config.distanceMax; // 数据5
   control.blocks[10].data6 = 0; // 数据6
   control.blocks[10].data7 = 0; // 数据7
   QByteArray temData = control.serialize();
   radarUDP->UDPSend(temData,remoteIP,remotePort);
   ui->LoggerTextEdit->append(QString("[INFO] %1 Control Cmd Send! \r\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
}

