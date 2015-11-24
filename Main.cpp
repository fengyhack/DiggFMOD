#include <fmod.hpp> // FMOD_...
#include <stdio.h>
#include <stdlib.h> // system()
#include <ctype.h> // isprint()
#pragma comment(lib,"fmod_vc.lib") // FMOD_xxx...
#pragma warning(disable:4996) 

void display(bool paused, bool dsp_echo, bool dsp_disto, float volume);

int main(void)
{
	/*
	下述的双冒号::表示全局作用域
	这样做是为了区分系统system与FMOD::System system
	参数color: 前景色/背景色
	参数title: 标题
	*/
	::system("color 2e");
	::system("title Simple FMOD demo by fengyhack");

	const char* fileName = "F:/1.flac";
	float volume = 0.5f;  // 初始音量50%

	FMOD::System* system = NULL;
	FMOD::Sound* sound = NULL;
	FMOD::Channel* channel = NULL;
	FMOD::DSP* dspEcho = NULL;  // 回声
	FMOD::DSP* dspDisto = NULL; // 扭曲
	System_Create(&system);
	system->init(10, FMOD_INIT_NORMAL, NULL); // 第一个参数10表示最多10个混音
	system->createSound(fileName, FMOD_LOOP_NORMAL | FMOD_2D, NULL, &sound);
	system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspEcho);
	system->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &dspDisto);

	// 现在设置初始状态如音量等
	channel->setVolume(volume);
	dspEcho->setBypass(true);
	dspDisto->setBypass(true);
	channel->addDSP(0, dspEcho);
	channel->addDSP(0, dspDisto);

	// !!!!!!!!!! 请务必执行update()以更新当前的设置
	system->update();
	system->playSound(sound, NULL, false, &channel);

	char key = 0;
	bool paused = false;
	bool dsp_echo = false;
	bool dsp_disto = false;
	while (true)
	{
		/*
		这个循环是为了让主函数永不结束(除非输入'Q'键)
		这样FMOD就有机会继续执行任务(播放声音)
		否则主函数会继续执行下一语句，直接退出
		实际情况不应该是这样，这里仅仅作为一个示例罢了
		实际情况应该是有一个前台任务在执行，在后台播放声音
		*/

		display(paused, dsp_echo, dsp_disto, volume);

		// !!!!!!!!!! 请务必执行update()
		system->update();

		key = getchar();
		if (isprint(key)) // 是否为可打印字符
		{
			/*
			再次执行getchar()以“吃掉”输入字符之后的回车符
			例如输入 X <ENTER>
			那么key=X然后吃掉<ENTER>
			如果不这么做,下一次循环执行key=getchar()
			结果为key=<ENTER> ('\n')
			*/
			getchar();
		}

		if (key == 'q' || key == 'Q') break;

		switch (key)
		{
		case 'P':
		case 'p':
			/*
			如何设置get和set的先后顺序?
			与update()有什么关系?
			*/
			channel->setPaused(!paused);
			channel->getPaused(&paused);
			break;
		case 'E':
		case 'e':
			dspEcho->getBypass(&dsp_echo);
			dspEcho->setBypass(!dsp_echo);
			break;
		case 'D':
		case 'd':
			dspDisto->getBypass(&dsp_disto);
			dspDisto->setBypass(!dsp_disto);
			break;
		case 'A':
		case 'a':
			volume += 0.1f;
			if (volume > 1.0) volume = 1.0f;
			channel->setVolume(volume);
			break;
		case 'B':
		case 'b':
			volume -= 0.1f;
			if (volume < 0.1) volume = 0.1f;
			channel->setVolume(volume);
			break;
		default:
			break;
		}
	}

	sound->release();
	system->close();

	return 0;
}

void display(bool paused, bool dsp_echo, bool dsp_disto, float volume)
{
	::system("cls"); // 清屏
	printf("---------------------------------\n");
	printf("| **** SOUND_VOL : %.1f **** \n", volume);
	if (dsp_echo)
	{
		printf("| **** DSP_ECHO  : ON  **** \n");
	}
	else
	{
		printf("| **** DSP_ECHO  : OFF **** \n");
	}
	if (dsp_disto)
	{
		printf("| **** DSP_DISTO : ON  **** \n");
	}
	else
	{
		printf("| **** DSP_DISTO : OFF **** \n");
	}

	printf("|\n");

	if (paused) // 暂停或继续
	{
		printf("| P/p: press to resume\n");
	}
	else
	{
		printf("| P/p: press to pause\n");
	}
	if (dsp_echo) // 回声效果
	{
		printf("| E/e: disable echo effect\n");
	}
	else
	{
		printf("| E/e: enable echo effect\n");
	}
	if (dsp_disto)  // 扭曲("破音")
	{
		printf("| D/d: disable distortion\n");
	}
	else
	{
		printf("| D/d: enable distortion\n");
	}

	printf("| A/a: volume +0.1 (max 1.0)\n");  // 音量增10%
	printf("| B/b: vloume -0.1 (min 0.1)\n");  // 音量减10%
	printf("| Q/q: stop and quit\n");  // 退出
	printf("---------------------------------\n");
	printf("Command (with ENTER):");
}
