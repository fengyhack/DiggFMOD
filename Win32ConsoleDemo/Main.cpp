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
	������˫ð��::��ʾȫ��������
	��������Ϊ������ϵͳsystem��FMOD::System system
	����color: ǰ��ɫ/����ɫ
	����title: ����
	*/
	::system("color 2e");
	::system("title Simple FMOD demo by fengyhack");

	const char* fileName = "F:/1.flac";
	float volume = 0.5f;  // ��ʼ����50%

	FMOD::System* system = NULL;
	FMOD::Sound* sound = NULL;
	FMOD::Channel* channel = NULL;
	FMOD::DSP* dspEcho = NULL;  // ����
	FMOD::DSP* dspDisto = NULL; // Ť��
	System_Create(&system);
	system->init(10, FMOD_INIT_NORMAL, NULL); // ��һ������10��ʾ���10������
	system->createSound(fileName, FMOD_LOOP_NORMAL | FMOD_2D, NULL, &sound);
	system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dspEcho);
	system->createDSPByType(FMOD_DSP_TYPE_DISTORTION, &dspDisto);

	// �������ó�ʼ״̬��������
	channel->setVolume(volume);
	dspEcho->setBypass(true);
	dspDisto->setBypass(true);
	channel->addDSP(0, dspEcho);
	channel->addDSP(0, dspDisto);

	// !!!!!!!!!! �����ִ��update()�Ը��µ�ǰ������
	system->update();
	system->playSound(sound, NULL, false, &channel);

	char key = 0;
	bool paused = false;
	bool dsp_echo = false;
	bool dsp_disto = false;
	while (true)
	{
		/*
		���ѭ����Ϊ������������������(��������'Q'��)
		����FMOD���л������ִ������(��������)
		���������������ִ����һ��䣬ֱ���˳�
		ʵ�������Ӧ�������������������Ϊһ��ʾ������
		ʵ�����Ӧ������һ��ǰ̨������ִ�У��ں�̨��������
		*/

		display(paused, dsp_echo, dsp_disto, volume);

		// !!!!!!!!!! �����ִ��update()
		system->update();

		key = getchar();
		if (isprint(key)) // �Ƿ�Ϊ�ɴ�ӡ�ַ�
		{
			/*
			�ٴ�ִ��getchar()�ԡ��Ե��������ַ�֮��Ļس���
			�������� X <ENTER>
			��ôkey=XȻ��Ե�<ENTER>
			�������ô��,��һ��ѭ��ִ��key=getchar()
			���Ϊkey=<ENTER> ('\n')
			*/
			getchar();
		}

		if (key == 'q' || key == 'Q') break;

		switch (key)
		{
		case 'P':
		case 'p':
			/*
			�������get��set���Ⱥ�˳��?
			��update()��ʲô��ϵ?
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
	::system("cls"); // ����
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

	if (paused) // ��ͣ�����
	{
		printf("| P/p: press to resume\n");
	}
	else
	{
		printf("| P/p: press to pause\n");
	}
	if (dsp_echo) // ����Ч��
	{
		printf("| E/e: disable echo effect\n");
	}
	else
	{
		printf("| E/e: enable echo effect\n");
	}
	if (dsp_disto)  // Ť��("����")
	{
		printf("| D/d: disable distortion\n");
	}
	else
	{
		printf("| D/d: enable distortion\n");
	}

	printf("| A/a: volume +0.1 (max 1.0)\n");  // ������10%
	printf("| B/b: vloume -0.1 (min 0.1)\n");  // ������10%
	printf("| Q/q: stop and quit\n");  // �˳�
	printf("---------------------------------\n");
	printf("Command (with ENTER):");
}
