#include "_func.h"
#include "Line.h"
#include "przystanek.h"
#include "KnotNet.h"
#include "_slaves.h"
#include "kombinacje.h"
#include "permutacja.h"
#include <queue>
#include <algorithm>


/*
Funkcja testowa
*/
void wyswietl(std::vector<kint<int> > v, int R)
{
	std::cout << std::endl;
	for (auto i = 0; i < R; i++)
	{
		std::cout << v[i].m_value;
	}
}
/*
Funkcja dzieląca zbiór na grupy o wspolnych przystankach 
*/
std::vector<Cgroup> divideIntoGroups(const CconnectionMatrix &matrix)
{
	std::vector<Cgroup> groups;
	bool ctrl;
	int rootId = 0;
	while (rootId < matrix.numOfLines())
	{
		groups.push_back(Cgroup(rootId, matrix));
		ctrl = true;
		while (ctrl && rootId < matrix.numOfLines())
		{
			int ctrlroot = rootId;
			for (unsigned int i = 0; i < groups.size(); i++)
			{
				if (onlyOnceRuleCtrl(rootId, groups[i].groupMembers()))//jesli jest skladowa jakiejs grupy
				{
					rootId++;
					break;
				}
			}
			if (ctrlroot == rootId)//jesli nie nalezy to stworz grupe z tym elementem
				ctrl = false;
		}
	}
	return groups;
}




//tu sie dzieje cala magia(rekurencji)
CKnotNet& syncMagic(const std::vector<CStop> &pq, const CconnectionMatrix &data, const Cgroup &group, CKnotNet &bestOption, CKnotNet thisOption, int version = 0)
{
	if (thisOption.isKnotGood(pq[version].id()))//wezel juz wypelniony z poprzednich
		return syncMagic(pq, data, group, bestOption, thisOption, version + 1);

	CKnotNet tmpOption(thisOption);
	const SviComb &combi = Ccombinations(pq[version].numOfLines(), 12/*to jako arg pory dnia*/, 3 /*tez arg programu*/).retComb();
	const std::vector<int> &lines = data.whichLineStopsHere(pq[version].id());
	Cpermutation<int,wyswietl> perm(lines, lines.size(), true);

	for each  (const auto &c in combi)
	{
		for each  (const auto &p in perm.retPermTab())
		{
			try
			{
				tmpOption.fill(c, p);
			}
			catch (int err_id)
			{
				if (err_id == 0)
					break;
				if (err_id == 1)
					continue;
				std::cerr << "ERR_UNEX_EXP_THROWN";
				std::cin.sync();
				std::cin.get();
				std::abort();
			}
			if (tmpOption.isGood())
			{
				if (tmpOption.finished())
				{
					if (bestOption.rating() < tmpOption.rating())
						bestOption = tmpOption;
				}
				else
					syncMagic(pq, data, group, bestOption, tmpOption, version + 1);
			}
			else
			{
				tmpOption = thisOption;
				continue;
			}
		}
	}
	return bestOption;
}






//sync grupy
const individual& groupSync(const CconnectionMatrix &data, const Cgroup &group)
{
	std::vector<CStop> pq = {};
	for each  (const auto &stop in group.whereLinesStops())//wypelnianie "kolejki"
	{
		const CStop &stopInfo = data.getStopInfo(stop);
		if (stopInfo.isItKnot())
		{

			pq.push_back(stopInfo);
		}
		else continue;
	}
	std::stable_sort(pq.begin(), pq.end(), KnotComp);
	CKnotNet bestOption;
	syncMagic(pq, data, group,bestOption,bestOption);
	return individual(bestOption);
}
//funkcja synchronizująca
const individual& sync(const CconnectionMatrix &data, const std::vector<Cgroup> &groups)
{
	individual result;
	for (auto i = 0u; i < groups.size(); ++i)
	{
		auto group = groups[i];
		result += groupSync(data, group);
	}
	return result;
}
