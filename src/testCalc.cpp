// testCalc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <string>



//вычисляет значение строкового выражения 
//выражение может содержать 0..9  ( ) -+ */
class evaluator
{

public:
    evaluator::evaluator()
    {

    }

    evaluator::~evaluator()
    {

    }

	//текст для выдячи в случае ошибки
    const std::string sError = "error";


	//пытается преобразовать часть строки в значение числа
    float toValue(std::string _strInData, std::size_t _posStart, std::size_t _posEnd,bool &_isError)
    {
        std::string sValue = _strInData.substr(_posStart, _posEnd - _posStart);
        try
        {
            _isError = false;
            return std::stof(sValue);
        }
        catch (...)
        {
            _isError = true;
            return 0;
        }
    }
	//метод разбора строки
	// выпоняется рекурсивно
	// выполняет операцию с наибольшим приоритетом, и заменяет соответствующую часть строки на результат, далее пытается вычислить чтото ещё рекурсивным вызовом
    std::string evaluator::calc(std::string _strInData)
    {
        


        try
        {
           
           

            int iErrorFlag = 0;
            iErrorFlag = 0;
            std::string sOut = "";
            //find ( ) and calc all of them
            std::size_t iSpecSymbolPos = _strInData.find_first_of("()");
            std::size_t iSpecSymbolPosX = iSpecSymbolPos;
            while (iSpecSymbolPos != std::string::npos)
            {
                int iOpenBracketCount = 0;
                int iCloseBracketCount = 0;
                if (_strInData[iSpecSymbolPos] == '(')
                {
                    iOpenBracketCount++;
                    while (iOpenBracketCount != iCloseBracketCount)
                    {
                        iSpecSymbolPosX = _strInData.find_first_of("()", iSpecSymbolPosX + 1);
                        if (iSpecSymbolPosX == std::string::npos)
                        {
                            //if end of string
                            sOut = sError;
                            iErrorFlag = 1;
                            return sOut;
                        }
                        else if (_strInData[iSpecSymbolPosX] == '(')
                        {
                            iOpenBracketCount++;
                        }
                        else if (_strInData[iSpecSymbolPosX] == ')')
                        {
                            iCloseBracketCount++;
                        }
                        else
                        {
                            //if unexpected error
                            sOut = sError;
                            iErrorFlag = 1;
                            return sOut;
                        }
                    }
                    std::string sPartEval = _strInData.substr(iSpecSymbolPos + 1, iSpecSymbolPosX - iSpecSymbolPos - 1);
                    std::string partVal = calc(sPartEval);

                    std::string sReplacedEval = _strInData.substr(0, iSpecSymbolPos);
                    sReplacedEval.append(partVal);
                    sReplacedEval.append(_strInData.substr(iSpecSymbolPosX + 1, _strInData.length() - iSpecSymbolPosX - 1));
                    sOut = calc(sReplacedEval);
                    return sOut;
                    long a = 1;
                }
                else
                {
                    //found unexpected ')'
                    sOut = sError;
                    iErrorFlag = 1;
                    return sOut;
                }

            }
            //there are no ().   only +-*/ and digit
            iSpecSymbolPos = _strInData.find_first_of("*/");
            if (iSpecSymbolPos != std::string::npos)
            {
                float valFirst = 0;
                float valSecond = 0;
                float calcVal = 0;
                std::size_t opStart = 0;
                std::size_t opEnd = 0;
                std::size_t iSpecSymbolPosX = _strInData.find_last_of("-+*/", iSpecSymbolPos - 1);
                if (iSpecSymbolPosX == std::string::npos)
                {
                    opStart = 0;
                    std::string sPart = _strInData.substr(0, iSpecSymbolPos);
                    valFirst = std::stof(sPart);
                }
                else
                {
                    opStart = iSpecSymbolPosX + 1;
                    std::string sPart = _strInData.substr(iSpecSymbolPosX + 1, iSpecSymbolPos - iSpecSymbolPosX - 1);
                    valFirst = std::stof(sPart);
                }

                std::size_t iSpecSymbolPosFirstDigit = _strInData.find_first_of("0123456789.", iSpecSymbolPos + 1);
                iSpecSymbolPosX = _strInData.find_first_of("-+*/", iSpecSymbolPosFirstDigit + 1);
                if (iSpecSymbolPosX == std::string::npos)
                {
                    opEnd = _strInData.length();
                    std::string sPart = _strInData.substr(iSpecSymbolPos + 1, opEnd - iSpecSymbolPos - 1);
                    valSecond = std::stof(sPart);
                }
                else
                {
                    
                    opEnd = iSpecSymbolPosX;
                    std::string sPart = _strInData.substr(iSpecSymbolPos + 1, opEnd - iSpecSymbolPos - 1);
                    valSecond = std::stof(sPart);
                }
                //calc * or / operation
                if (_strInData[iSpecSymbolPos] == '*')
                {
                    calcVal = valFirst*valSecond;
                }
                else if (_strInData[iSpecSymbolPos] == '/')
                {
                    //TODO - error div by 0
                    calcVal = valFirst / valSecond;

                }
                else
                {
                    //found unexpected error
                    sOut = sError;
                    iErrorFlag = 1;
                    return sOut;
                }

                //do other operations
                std::string partVal = std::to_string(calcVal);


                std::string sReplacedEval = _strInData.substr(0, opStart);
                sReplacedEval.append(partVal);
                sReplacedEval.append(_strInData.substr(opEnd, _strInData.length() - opEnd));
                sOut = calc(sReplacedEval);
                return sOut;

            }

            //only -+ special symbol

            //minus can be on first pos - as sign of value - to solve difference between -sign and -operation
            iSpecSymbolPos = _strInData.find_first_of("-+", 0);
            size_t iSpecSymbolPosSecond = _strInData.find_first_of("-+", 1);
            if (iSpecSymbolPos > 0)
                iSpecSymbolPos = std::string::npos;
            if (iSpecSymbolPosSecond > 1)
                iSpecSymbolPosSecond = std::string::npos;
            if (iSpecSymbolPos != std::string::npos && (iSpecSymbolPosSecond != std::string::npos))
            {
                
                if (_strInData[iSpecSymbolPos] != _strInData[iSpecSymbolPosSecond])
                {
                    std::string sReplacedEval = "-";
                    sReplacedEval.append(_strInData.substr(2, _strInData.length() - 2));
                    sOut = calc(sReplacedEval);
                    return sOut;

                }
                else
                {

                    std::string sReplacedEval = "";
                    sReplacedEval.append(_strInData.substr(2, _strInData.length() - 2));
                    sOut = calc(sReplacedEval);
                    return sOut;

                }
            }
            iSpecSymbolPos = _strInData.find_first_of("-+", 1);
            if (iSpecSymbolPos != std::string::npos)
            {
                float valFirst = 0;
                float valSecond = 0;
                float calcVal = 0;
                std::size_t opStart = 0;
                std::size_t opEnd = 0;

                std::size_t iSpecSymbolPosX = _strInData.find_last_of("-+*/", iSpecSymbolPos - 1);
                if (iSpecSymbolPosX == std::string::npos)
                {
                    opStart = 0;
                    std::string sPart = _strInData.substr(0, iSpecSymbolPos);
                    valFirst = std::stof(sPart);
                }
                else
                {
                    opStart = iSpecSymbolPosX + 1;
                    std::string sPart = _strInData.substr(iSpecSymbolPosX + 1, iSpecSymbolPos - iSpecSymbolPosX - 1);
                    valFirst = std::stof(sPart);
                }

                std::size_t iSpecSymbolPosFirstDigit = _strInData.find_first_of("0123456789.", iSpecSymbolPos + 1);
                iSpecSymbolPosX = _strInData.find_first_of("-+*/", iSpecSymbolPosFirstDigit + 1);
                if (iSpecSymbolPosX == std::string::npos)
                {
                    opEnd = _strInData.length();
                    std::string sPart = _strInData.substr(iSpecSymbolPos + 1, opEnd - iSpecSymbolPos - 1);
                    valSecond = std::stof(sPart);
                }
                else
                {
                    opEnd = iSpecSymbolPosX ;
                    std::string sPart = _strInData.substr(iSpecSymbolPos + 1, opEnd - iSpecSymbolPos - 1);
                    valSecond = std::stof(sPart);
                }
                //calc + or - operation
                if (_strInData[iSpecSymbolPos] == '+')
                {
                    calcVal = valFirst+valSecond;
                }
                else if (_strInData[iSpecSymbolPos] == '-')
                {
                   
                    calcVal = valFirst - valSecond;

                }
                else
                {
                    //found unexpected error
                    sOut = sError;
                    iErrorFlag = 1;
                    return sOut;
                }

                //do other operations
                std::string partVal = std::to_string(calcVal);


                std::string sReplacedEval = _strInData.substr(0, opStart);
                sReplacedEval.append(partVal);
                sReplacedEval.append(_strInData.substr(opEnd, _strInData.length() - opEnd));
                sOut = calc(sReplacedEval);
                return sOut;

            }


            //suppose only one value in string at moment
            iSpecSymbolPos = _strInData.find_first_of("-+*/",1);
            if (iSpecSymbolPos != std::string::npos)
            {
                sOut = sError;
                iErrorFlag = 1;
                return sOut;
            }
            else
            {
                bool isError;
                sOut = std::to_string(toValue(_strInData, 0, _strInData.length(), isError));
                if (isError)
                {
                    sOut = sError;
                    iErrorFlag = 1;
                    return sOut;
                }
                else
                {
                    return sOut;
                }
            }



         
            //unreachable
            return sError;
        }
        catch (...)
        {
            return sError;
        }
    }

   
};






void checkData(std::string _wantData, std::string _actualData,std::string _inData)
{
    int iShowRight = 0;
    int isSame = 0;
    if (strcmp(_wantData.c_str(), _actualData.c_str()) == 0)
    {
        isSame = 1;
    }
    try
    {
        float valWant = std::stof(_wantData);
        float valActual = std::stof(_actualData);
        if (fabs(valWant - valActual) < 0.0000001)
        {
            isSame = 1;
        }
    }
    catch (...)
    {
    }
    


    if (isSame)
    {
        if (iShowRight)
            printf("%s = %s (%s)\n", _inData.c_str(), _actualData.c_str(), _wantData.c_str());
    }
    else 
    {
        printf("%s = %s (%s)\n", _inData.c_str(), _actualData.c_str(), _wantData.c_str());
    }


}
void runTestAll()
{
    std::string sInData;
    std::string sOutData;
    std::string sWantData;

    evaluator thisEval;
    sWantData = "-1"; sInData = "1-2"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "2"; sInData = "-1*-2"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "-0.5"; sInData = "-1/2"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);


    sWantData = "-2"; sInData = "-2"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "-2"; sInData = "1*-2"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);

    sWantData = "7"; sInData = "1+2+4"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "3"; sInData = "1+2"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);

    sWantData = "1.23"; sInData = "1.23"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = thisEval.sError; sInData = "ab3"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "11"; sInData = "(11)"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "11"; sInData = "(((11)))"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = thisEval.sError;; sInData = "((11)"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "2"; sInData = "1*2"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "6"; sInData = "1*(2*3)"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "2"; sInData = "1*((2/3)*3)"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
//    sWantData = thisEval.sError; sInData = "1*(2/0)"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = thisEval.sError; sInData = "1*"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "6"; sInData = "1+2*3-1"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "5.66"; sInData = "1+2/3+4"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);


    sWantData = "3"; sInData = "1+2"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = thisEval.sError; sInData = "1+"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "11"; sInData = "1+2*3+4"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "6.28"; sInData = "1+2/(3+4)+5"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = thisEval.sError; sInData = "1+2/((3+4)+5"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "10.05"; sInData = "1+2/(((((3+4)*5)+1)+2))+9"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
    sWantData = "-0.28"; sInData = "1+(1.2*3.1)-5"; sOutData = thisEval.calc(sInData); checkData(sWantData, sOutData, sInData);
}
int _tmain(int argc, _TCHAR* argv[])
{
    runTestAll();


    char cc = getchar();
	return 0;
}

