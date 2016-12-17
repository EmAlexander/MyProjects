#include "AvCfg.h"

AvCfg<void>::AvCfg()
{
}

AvCfg<void>::~AvCfg()
{
}

int AvCfg<void>::avCfg_start(string fileStr, cfgFileTyp fTyp)
{
	cfg_file.open(fileStr.c_str(), ios::in);
	if (cfg_file.is_open())
	{
		if (fTyp == CFG_F_ENCRYP)
		{
			encrypt = true;
			return 0;
		}
		else if (fTyp == CFG_F_UNENCR)
		{
			encrypt = false;
			return 0;
		}
		else
		{
			cout << "Invalid fTyp!" << endl;
			failed = true;
			return -1;
		}
	}
	cout << "Config file does not exist!" << endl;
	failed = true;
	return -1;
}

void AvCfg<void>::avCfg_stop()
{
	cfg_file.close();
}