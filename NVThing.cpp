#include "NVThing.h"
#include "Exception.h"
#include <sstream>

using namespace Common;


const char* NVThing::sCVersion("NVT_1.0");

NVThing::NVThing(int aILevel) 
{ 
	mILevel = aILevel; 
}

NVThing::NVThing(const NVThing &aNvt)
{
	operator=(aNvt);
}

NVThing::~NVThing()
{
	Clear();
}

NVThing& NVThing::operator=(const NVThing &aNvt)
{
	Clear();

	NVMapType::const_iterator lIter = aNvt.mMValues.begin();
	NVMapType::const_iterator lIterEnd = aNvt.mMValues.end();
	while (lIter != aNvt.mMValues.end())
	{
		if (lIter->second->mS)
		{
			// second is a string.
			Add(lIter->first.c_str(), lIter->second->mS->c_str());
		}
		else
		{
			// second is an NVT.  duplicate it.
			NVThing *lNvt = new NVThing;
			*lNvt = *(lIter->second->mNvt);
			Add(lIter->first.c_str(), lNvt);
		}
		++lIter;
	}

	return *this;
}

void NVThing::Clear()
{
	mIter = mMValues.begin();
	while (mIter != mMValues.end())
	{
		delete mIter->second;
		++mIter;
	}
	mMValues.clear();
}

void NVThing::ToOs(ostream &aOs) const
{
	aOs << sCVersion << "\n";
	ToOs_Internal(aOs);
}

void NVThing::ToOs_Internal(ostream &aOs) const
{
	string lSTabs("");

	for (int lI = 0; lI < mILevel; ++lI)
	{
		lSTabs += "\t";
	}

	// Write all the Name-Value pairs out.
	NVMapType::const_iterator lIter = mMValues.begin();
	while (lIter != mMValues.end())
	{
		aOs << lSTabs.c_str() << lIter->first << "=";
		if (lIter->second->mS)
		{
			StringOut(aOs, *(lIter->second->mS));
			aOs << "\n";
		}
		else
		{
			lIter->second->mNvt->mILevel = this->mILevel + 1;
			aOs << "=" << "\n";
			lIter->second->mNvt->ToOs_Internal(aOs);
		}

		++lIter;
	}
}

void NVThing::FromIs(istream &aIs)
{
	// Support the legacy NVT.
	string lS;
	aIs >> lS;
	if (lS == sCVersion)
	{
		// skip to the next line.
		char lC[10];
		aIs.getline(lC, 10);

		FromIs_Internal(aIs);
	}
	else
	{
		aIs.seekg(0);
		FromIs_Old(aIs);
	}
}

void NVThing::FromIs_Internal(istream &aIs)
{
	Clear();

	ostringstream lOss;

	while (true)
	{
		// Get the data line.
		streamoff lSpName = aIs.tellg();

		if (lSpName == -1)
			break;

		// Get a line.
		lOss.str("");
		aIs.get(*(lOss.rdbuf()));
		aIs.get();
		
		const string &lSData(lOss.str());
		
		if (aIs.eof())
			break;
			
		// skip blank lines (tabs or spaces only)
		if (lSData.find_first_not_of(" \t") == -1)
		{
			// try getting another char if it was empty.
			if (lSData == "")
			{
				// reset fail flag!
				aIs.clear();
				aIs.get();
			}
		}
		else
		{
			// Count the tabs.  Tabs indicate the 'level' aka sub-object depth.
			// In other words, when one NVThing is embedded in another, its data will
			// be written out indented with one more tab than its parent.
	        int lITabs;
			for (lITabs = 0; lITabs < 1024; ++lITabs)
			{
				if (lSData[lITabs] != '\t')
					break;
			}
	
			if (lITabs > mILevel)
			{
				throw MessageException("Bad Tab Count", MultiChar("bdtb"));
			}
			else if (lITabs < mILevel)
			{
				// We're done with this level, this name is for a 'lower' level object.
				// rewind aIs and return.
				aIs.seekg(lSpName);
				return;
			}
	
			// Value starts after the first "=".  
			size_t lIEq(lITabs);
			for (; lSData[lIEq] != '='; ++lIEq)
			{
				if (lSData[lIEq] == '\0')
					throw MessageException("missing '=' in name-value pair!", MultiChar("bdeq"));
			}
	
			++lIEq;
	
			// If the next char is an '=', then the value part is a sub-NVT.
			if (lSData[lIEq] == '=')
			{
				// This is really the name of a var in a higher level object.
				NVThing *lNvt = new NVThing(mILevel + 1);
	
				// Read in the NVT starting at the next line in the file.
				lNvt->FromIs_Internal(aIs);
	
				// back up to the first '=' again.
				--lIEq;
				Add(lOss.str().substr(lITabs, lIEq - lITabs).c_str(), lNvt);
			}
			else
			{
				// lIEnd points to one past the last char.
				size_t lIEnd = lSData.size();
				if (lSData[lIEnd - 1] == '\r')
					--lIEnd;
	
				Add(lSData.substr(lITabs, lIEq - 1 - lITabs).c_str(),
					lSData.substr(lIEq, lIEnd - lIEq));
			}
		}
	}
}

void NVThing::FromIs_Old(istream &aIs)
{
	Clear();

	// Read in the Name-Value pairs, and sub-NVThings.
	char lCName[1024];
	char lCValue[1024];

	while (true)
	{
		// Get the Name line.

		streamoff lSpName = aIs.tellg();

		if (lSpName == -1)
			break;

		aIs.getline(lCName, 1024);

		if (aIs.eof())
			break;

		// Count the tabs.
        int lITabs;
		for (lITabs = 0; lITabs < 1024; ++lITabs)
		{
			if (lCName[lITabs] != '\t')
				break;
		}

		if (lITabs > mILevel)
		{
			throw MessageException("Bad Tab Count", MultiChar("bdtb"));
		}
		else if (lITabs < mILevel)
		{
			// We're done with this level, this name is for a 'lower' level object.
			// rewind aIs and return.
			aIs.seekg(lSpName);
			return;
		}

		streamoff lSpValue = aIs.tellg();
		streamoff lSpWk = lSpValue;
#ifdef WIN32
		lSpWk -= lSpName + 2;  // CRLF
#else
        lSpWk -= lSpName + 1;  // LF
#endif
		// Write over the final \n with a \0.
		lCName[lSpWk] = '\0';
		// Get the Value line.
		aIs.getline(lCValue, 1024);

		// Count the tabs.
		for (lITabs = 0; lITabs < 1024; ++lITabs)
		{
			if (lCValue[lITabs] != '\t')
				break;
		}

		if (lITabs == mILevel)
		{
			// Write over the final \n with a \0.
			//lCValue[aIs.tellg() - lSpValue - 2] = '\0';
			lSpWk = aIs.tellg();
#ifdef WIN32
            lSpWk -= lSpValue + 2;  // CRLF
#else
			lSpWk -= lSpValue + 1;  // LF
#endif
			lCValue[lSpWk] = '\0';
			// Add the name and value, sans tabs.
			Add(lCName + mILevel, lCValue + mILevel);
		}
		else if (lITabs > mILevel)
		{
			// This is really the name of a var in a higher level object.
			NVThing *lNvt = new NVThing(mILevel + 1);

			// Rewind to point at the beginning of the value line.
			aIs.seekg(lSpValue);

			lNvt->FromIs_Old(aIs);

			Add(lCName, lNvt);
		}
		else 
		{
			// If less, then value level doesn't match name level.
			throw MessageException("Bad Tab Count", MultiChar("bdtb"));
		}
	}
}

/*
void NVThing::ToXml(ostream &aOs) const
{
	TiXmlDocument lTxd;
	ToXml(lTxd);
	aOs << lTxd;
}

void NVThing::FromXml(istream &aIs)
{
	TiXmlDocument lTxd;
	aIs >> lTxd;
	FromXml(lTxd);
}

void NVThing::ToXml(TiXmlNode &aTxn) const
{
	// Write all the Name-Value pairs out.
	NVMapType::const_iterator lIter = mMValues.begin();
	while (lIter != mMValues.end())
	{
		TiXmlElement *lTxe = new TiXmlElement(lIter->first);

		if (lIter->second->mS)
		{
			TiXmlText *lTxt = new TiXmlText(*(lIter->second->mS));
			lTxe->LinkEndChild(lTxt);
			//lTxt = new TiXmlText("testing");
			//lTxe->LinkEndChild(lTxt);
		}
		else
		{
			lIter->second->mNvt->ToXml(*lTxe);
		}
		aTxn.LinkEndChild(lTxe);

		++lIter;
	}
}

void NVThing::FromXml(const TiXmlNode &aTxn)
{
	// Assume NVThing is top level.  It contains elements.
	// If the elements contain only text, then they are data items.  
	// If the elements do not contain text, then they are sub-NVT
	const TiXmlNode *lTxn = aTxn.FirstChild();
	if (!lTxn)
		return;

	while (lTxn)
	{
		if (lTxn->Type() != TiXmlNode::ELEMENT)
			throw MessageException("XML format error");

		const TiXmlNode *lTxnSub = lTxn->FirstChild();
		if (lTxnSub)
		{
			switch (lTxnSub->Type())
			{
			case TiXmlNode::TEXT:
				// If this is a text node, then lTxnSub is a name-value pair.
				Add(lTxn->Value(), lTxnSub->ToText()->Value());
				break;
			case TiXmlNode::ELEMENT:
				// lTxn is a container.  We'll want to recursively call FromXml on lTxn.
				{
					NVThing *lNvtSub = new NVThing;
					lNvtSub->FromXml(*lTxn);
					Add(lTxn->Value(), lNvtSub);
				}
				break;
			}
		}
		// Advance to the next xml node.
		lTxn = lTxn->NextSibling();
	}
}
*/

	/*
	// Test NVThing.

	NVThing lNvt;
	lNvt.Add("test0", "value");
	lNvt.Add("test1", "value");
	lNvt.Add("test2", "value");

	NVThing lNvt2;
	lNvt2.Add("Hmm", "Homm");
	lNvt2.Add("Hmmty", "Hommty");

	lNvt.Add("Heh", &lNvt2);

	ofstream lOfs("NVTEst.txt");
	lOfs << lNvt;
	lOfs.close();
	
	NVThing lNvt3;
	ifstream lIfs("NVTEst.txt");
	lIfs >> lNvt3;
	lIfs.close();

	lOfs.open("NVTEst2.txt");
	lOfs << lNvt3;
	lOfs.close();

	NVThing *lNvtWk;
	if (lNvt3.Find("Heh", lNvtWk))
	{
		const char *lC;
		lNvtWk->Find("Hmmty", lC);
		lNvt3.Find("test1", lC);
	}
	*/

	/*
	{
		// Add/Remove things test.
		NVThing lNvt;
		lNvt.Add("test", "string");
		lNvt.Add("test1", 1);
		lNvt.Add("test2", 2.0);
		int64_t lI64 = 5000000000000000;
		lNvt.Add("test3", lI64);

		int lI;
		double lD; 
		lI64 = 1;
		const char *lC;
		bool lB;
		lB = lNvt.Find("test", lC);
		lB = lNvt.Find("test1", lI);
		lB = lNvt.Find("test2", lD);
		lB = lNvt.Find("test3", lI64);
	}
	*/
