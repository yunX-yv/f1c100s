- [ 0.说明](#head1)
	- [ 芯片介绍](#head2)
	- [ 开发工具](#head3)



## <span id="head1"> 0.说明</span>

> 本项目是一个基于全志F1C100s芯片的超迷你&低成本的Linux开发板.
>
> **板载资源：**
>
> * 一个SPI-LCD
> * 一个麦克风 & 功放可以外接喇叭
> * 一个板载麦克风
> * 一个USB-Type-C
> * SD卡插槽
> * 一个IIC温湿度
> * 一个SPI-flsah
> * WIFI(基于ESP8089 SPI)
>

### <span id="head2"> 芯片介绍</span>

> 全志F1C100s&F1C200s是全志的一款高度集成、低功耗的移动应用处理器，可用于多种多媒体音视频设备中。

`F1C200s`基于ARM 9架构，芯片集成了SiP的DDR，外围电路可以极其简单；它支持高清视频解码，包括H.264、H.263、MPEG 1/2/4等，还集成了音频编解码器和I2S/PCM接口，是一款开发简单、性价比较高的产品，也适合用来做入门级的Linux开发板。

---

| 功能         | 描述                                                         |
| ------------ | ------------------------------------------------------------ |
| CPU          | ARM9 CPU architecture，16KByte D-Cache，2KByte I-Cache       |
| Memory       | SIP 64MB DDR1，SD2.0，eMMC 4.41                              |
| Video        | H.264 1920x1080@30fps decoding<br/>MPEG1/2/4 1920x1080@30fps decoding<br/>MJPEG 1280x720@30fps encoding<br/>JPEG encode size up to 8192x8192 |
| Camera       | 8-bit CMOS-sensor interface<br/>CCIR656 protocol for NTSC and PAL |
| Audio        | Integrated analog audio codec with two DAC channels and one ADC channel,maximum 192kHz DAC sample rate and 48kHz ADC sample rate<br/>One I2S/PCM interface |
| Display      | LCD RGB interface up to 1280x720@60fps<br/>TV CVBS output, support NTSC/PAL, with auto plug detecting |
| Connectivity | USB OTG, SDIO,IR, 3 x TWI, 2 x SPI, 3 x UART                 |
| OS           | Melis, Linux OS                                              |
| Package      | QFN88, 10mm x 10mm                                           |
| Process      | 40nm                                                         |
| 特点         | 支持H.264 1920x1080@30fps 解码<br/>支持MJPEG 1280x720@30fps 编码<br/>集成 64MB DDR1,集成音频CODEC<br/>低成本，低功耗，开发简单 |

![20220401221925](https://pengzhihui-markdown.oss-cn-shanghai.aliyuncs.com/img/20220405172955.png)**与其他系列芯片的对比：**

![image-20220401222336539](https://pengzhihui-markdown.oss-cn-shanghai.aliyuncs.com/img/20220401222336.png)

### <span id="head3"> 开发工具</span>
![alt text](image/image.png)**芯片启动流程**

#### <span id="head4">2. 开发工具链下载</span>

> 编译工具链官网：https://www.linaro.org/
>
> 或[Arm GNU Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain)，以linaro为例：进入`support->downloads`可以看到下载页面，点击`GNU cross-toolchain binary archives`，可以进入对应[下载列表](https://releases.linaro.org/components/toolchain/binaries/)，可以看到各个版本的toolchain，这里我使用的`latest-7/arm-linux-gnueabi/`即`gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabi`即可。
>

## <span id="head5"> 1.硬件开发</span>

### 其他参考资料

* [peng-zhihui/Planck-Pi](https://github.com/peng-zhihui/Planck-Pi)
* [全志F1C100s使用记录：资料索引与基础说明](https://blog.csdn.net/Naisu_kun/article/details/122704052)
* [荔枝派Nano 全流程指南](https://wiki.sipeed.com/soft/Lichee/zh/Nano-Doc-Backup/index.html)