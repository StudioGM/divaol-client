
#ifndef DIVARANKRESULT_H
#define DIVARANKRESULT_H

namespace divacore
{
	class DivaRankResult
	{
	public:
		enum RankResult
		{
			Misstake,
			Cheap,
			Standard,
			Great,
			Excellent,
			Perfect,
			Autolike,
			BeyondAuto
		};

		const float StandardLeast = 0.75;
		const float GreatLeast = 0.85;
		const float ExcellentLeast = 0.92;
		const float PerfectLeast = 1;
		

		static RankResult GetRankResult(bool isOver, int HP, int MaxCT, int MaxCombo, int Cool, int Fine, int Safe, int Bad, int Worst)
		{
			if(!isOver || HP <=0)
				return Misstake;

			int TotalNum = Cool + Fine + Safe + Bad + Worst;
			int CoolFineNum = Cool + Fine;

			if(CoolFineNum == TotalNum)
			{
				if(Cool==TotalNum && MaxCT>=6)
					return BeyondAuto;
				else if(Cool==TotalNum)
					return Autolike;
				else
					return Perfect;
			}
			else if(CoolFineNum >= (int)((float)TotalNum*ExcellentLeast))
				return Excellent;
			else if(CoolFineNum >= (int)((float)TotalNum*GreatLeast))
				return Great;
			else if(CoolFineNum >= (int)((float)TotalNum*StandardLeast))
				return Standard;
			else if(HP>0)
				return Cheap;

			return Misstake;
		}

	};




}



#endif