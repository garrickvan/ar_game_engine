/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "Button.h"


EGSound EGCButton::m_cSound;


EGCButton::EGCButton():bHasText(FALSE),bEnable(TRUE),
				m_iTextStyle(BTN_TEXT_CENTER),color1(m_sColor)
{
	color2=EGSColor(0.0f,1.0f,0.0f,1.0f);
	m_sTextColor=EGSColor(0.0f,1.0f,0.0f,1.0f);
}


/**\brief
 * 从INI文件载入的按钮数据
 */
BOOL EGCButton::LoadIni(char* _filename,char* _index)
{
	EGCIniFile Ini(_filename);
	
	m_sRect.x=Ini.ReadInt(_index,"x");
	m_sRect.y=Ini.ReadInt(_index,"y");
	m_sRect.width=Ini.ReadInt(_index,"width");
	m_sRect.height=Ini.ReadInt(_index,"height");

	color1.r=PERSENT(Ini.ReadInt(_index,"r"));
	color1.g=PERSENT(Ini.ReadInt(_index,"g"));
	color1.b=PERSENT(Ini.ReadInt(_index,"b"));
	color1.a=PERSENT(Ini.ReadInt(_index,"a"));

	m_chBackTexFile =Ini.ReadText(_index,"BackTexFile");

	bHasText	=Ini.ReadInt(_index,"bHasText");
	m_bBlend	=Ini.ReadInt(_index,"bBlend");
	bEnable		=Ini.ReadInt(_index,"bEnable");
	strcpy(m_chText,Ini.ReadText(_index,"text"));

	if(m_chBackTexFile!=NULL)	this->LoadTexture(m_chBackTexFile,1,m_bBlend);
	if(bHasText==TRUE)		this->SetText(m_chText);

	return TRUE;
}



/**\brief
 * 绘制循环,虚函数，由其父类EGWindow的draw()来调用
 */
void EGCButton::OnDraw(void)
{
	if(bEnable==FALSE)//按钮不可用时为灰色
	{
		this->SetColor(EGSColor(0.5f,0.5f,0.5f,1.0f));
		if(bHasText==TRUE)
		{
			SYS_FONT->SetColor(EGSColor(0.0f,0.0f,0.0f,1.0f));
		}
		m_iState=BUTTON_DISABLE;
	}
	else if(bEnable==TRUE)//按钮可用时
	{
		if(IsActive()==TRUE)//光标在按钮上时
		{
			this->SetColor(color2);
			if(bHasText==TRUE)
			{
				SYS_FONT->SetColor(!m_sTextColor);
			}

			if(EGInput::GetSysInput()->FirstMouseDownL())
			{
				if(EGMusic::GetSysMusic()->GetMusic()) m_cSound.LoadSoundFX(DOWNL_SOUND_NAME);
				if(EGMusic::GetSysMusic()->GetMusic()) m_cSound.Play();
			}

			if(EGInput::GetSysInput()->Clicked())	m_iState=BUTTON_CLICK;
			else if(SYS_MOUSE->ButtonDown(0))		m_iState=BUTTON_DOWNL;
			else if(SYS_MOUSE->ButtonDown(1))		m_iState=BUTTON_DOWNR;
			else									m_iState=BUTTON_ACTIVE;
		}
		else//通常情况
		{
			this->SetColor(color1);
			if(bHasText==TRUE)
			{
				SYS_FONT->SetColor(m_sTextColor);
			}
			m_iState=BUTTON_NORMAL;
		}
	}
	
	if(bHasText==TRUE)
	{
		SYS_FONT->Format(m_chText);
		if(m_iTextStyle==BTN_TEXT_LEFT)
			SYS_FONT->PrintAt(m_sRect.x+2,m_sRect.y+((m_sRect.height-SYS_FONT->GetHeight())>>1));
		else if(m_iTextStyle==BTN_TEXT_CENTER)
			SYS_FONT->PrintAt(m_sRect.x+((m_sRect.width-SYS_FONT->GetWidth())>>1),m_sRect.y+((m_sRect.height-SYS_FONT->GetHeight())>>1));
		else //if(m_iTextStyle==BTN_TEXT_RIGHT)
			SYS_FONT->PrintAt(m_sRect.x-2+(m_sRect.width-SYS_FONT->GetWidth()),m_sRect.y+((m_sRect.height-SYS_FONT->GetHeight())>>1));
	}

}
