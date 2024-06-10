#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>

template<typename TYPE>
void PrintVector(std::string VectorName, std::vector<TYPE> &PrintableVector){
    std::cout << VectorName << ":" << std::endl;
    for(TYPE i: PrintableVector)
        std::cout << i << " ";
    std::cout << std::endl;
}

bool IsNumber(std::string str){
    for(char i : str)
        if(i != '0' && i != '1' && i != '2' && i != '3' && i != '4' && i != '5' && i != '6' && i != '7' && i != '8' && i != '9' && i != '-' && i != '+')
            return false;
    return true;
}

class Dual{
    public:
        long double Value {}, Derivative{};
        
        Dual(long double V, long double D) : Value(V), Derivative(D) {
            
        }
        
        Dual(Dual const &DualNumber) : Value(DualNumber.Value), Derivative(DualNumber.Derivative){
            
        }
        
        Dual(){
            
        }
        
        void AssignDualNumber(long double V, long double D){
            Value = V;
            Derivative = D;
        }
};

class EvaluateFunctions{
    private:
        std::string Function;
        std::vector<Dual> DualVector {};
        size_t NumberOfDualNumbers {};
		char LeftDualOperand {};
		char RightDualOperand {};
		int LeftDualOperandPos {};
		int RightDualOperandPos {};
		int NextOperatorIndex {};
		int LeftParenthesesPos {};
		int RightParenthesesPos {};
		std::string ParentheticFunction {};
		bool HasNumberInOperation {false};
		std::string sin {"sin"};
		std::string cos {"cos"};
		std::string tan {"tan"};
		std::string cot {"cot"};
		std::string cosec {"csc"};
		std::string sec {"sec"};
		bool InParentheticFunction = false;
        std::ostringstream oss;
		char CurrentUnusedVariable {};
    public:
        EvaluateFunctions(std::vector<Dual> DV) : DualVector(DV) {
            NumberOfDualNumbers = DualVector.size();
			CurrentUnusedVariable = 97 + NumberOfDualNumbers;
        }
        
		EvaluateFunctions(std::string F, std::vector<Dual> DV) : Function(F), DualVector(DV) {
            NumberOfDualNumbers = DualVector.size();
			CurrentUnusedVariable = 97 + NumberOfDualNumbers;
		}
		
        void InputFunction(){
            std::cout << "Type the function: ";
            std::cin >> Function;
        }
        
        void Evaluate(){
			for(int i = 0; i < static_cast<int>(Function.size()); i++){
				auto MathFunctionIt = std::search(Function.begin(), Function.end(), sin.begin(), sin.end());
				if(MathFunctionIt != Function.end()){
					int MathFunctionIndex = MathFunctionIt - Function.begin();
					char Result = EvaluateSin(MathFunctionIndex);
					std::string Result_s {Result};
					UpdateFunction(MathFunctionIndex, MathFunctionIndex + 3, Result_s);
				}
				
				MathFunctionIt = std::search(Function.begin(), Function.end(), cos.begin(), cos.end());
				if(MathFunctionIt != Function.end()){
					int MathFunctionIndex = MathFunctionIt - Function.begin();
					char Result = EvaluateCos(MathFunctionIndex);
					std::string Result_s {Result};
					UpdateFunction(MathFunctionIndex, MathFunctionIndex + 3, Result_s);
				}
				
				MathFunctionIt = std::search(Function.begin(), Function.end(), tan.begin(), tan.end());
				if(MathFunctionIt != Function.end()){
					int MathFunctionIndex = MathFunctionIt - Function.begin();
					char Result = EvaluateTan(MathFunctionIndex);
					std::string Result_s {Result};
					UpdateFunction(MathFunctionIndex, MathFunctionIndex + 3, Result_s);
				}
				
				MathFunctionIt = std::search(Function.begin(), Function.end(), cot.begin(), cot.end());
				if(MathFunctionIt != Function.end()){
					int MathFunctionIndex = MathFunctionIt - Function.begin();
					char Result = EvaluateCot(MathFunctionIndex);
					std::string Result_s {Result};
					UpdateFunction(MathFunctionIndex, MathFunctionIndex + 3, Result_s);
				}
				
				MathFunctionIt = std::search(Function.begin(), Function.end(), cosec.begin(), cosec.end());
				if(MathFunctionIt != Function.end()){
					int MathFunctionIndex = MathFunctionIt - Function.begin();
					char Result = EvaluateCosec(MathFunctionIndex);
					std::string Result_s {Result};
					UpdateFunction(MathFunctionIndex, MathFunctionIndex + 3, Result_s);
				}
				
				MathFunctionIt = std::search(Function.begin(), Function.end(), sec.begin(), sec.end());
				if(MathFunctionIt != Function.end()){
					int MathFunctionIndex = MathFunctionIt - Function.begin();
					char Result = EvaluateSec(MathFunctionIndex);
					std::string Result_s {Result};
					UpdateFunction(MathFunctionIndex, MathFunctionIndex + 3, Result_s);
				}
			}

			for(int i = 0; i < static_cast<int>(Function.size()); i++){
				if(Function.at(i) == '('){
					char Result = EvaluateParentheses(i);
					std::string Result_S {Result};
					UpdateFunction(LeftParenthesesPos, RightParenthesesPos, Result_S);
					i = 0;
				}
				
				if(Function.at(i) == '^'){
					char Result = EvaluateExponent(i);
					std::string Result_s {Result};
					if(NextOperatorIndex == 0)
							NextOperatorIndex = Function.size();
					UpdateFunction(i - 1, NextOperatorIndex - 1, Result_s);
					i = 0;
				}
				
				if(Function.at(i) == '*'){
					DualArithmeticWithNumberEvaluator(i);
					if(HasNumberInOperation){
						i = 0;
						HasNumberInOperation = false;
						continue;
					}
					FindDualOperands(i);
					char Result = PerformMultiplication();
					std::string Result_s {Result};
					UpdateFunction(LeftDualOperandPos, RightDualOperandPos, Result_s);
					i = 0;
				}
				
				if(Function.at(i) == '/'){
					DualArithmeticWithNumberEvaluator(i);
					if(HasNumberInOperation){
						i = 0;
						HasNumberInOperation = false;
						continue;
					}
					FindDualOperands(i);
					char Result = PerformDivision();
					std::string Result_s {Result};
					UpdateFunction(LeftDualOperandPos, RightDualOperandPos, Result_s);
					i = 0;
				}
				
				if(Function.at(i) == '+'){
					DualArithmeticWithNumberEvaluator(i);
					if(HasNumberInOperation){
						i = 0;
						HasNumberInOperation = false;
						continue;
					}
					FindDualOperands(i);
					char Result = PerformAddition();
					std::string Result_s {Result};
					UpdateFunction(LeftDualOperandPos, RightDualOperandPos, Result_s);
					i = 0;
				}
				
				if(Function.at(i) == '-'){
					DualArithmeticWithNumberEvaluator(i);
					if(HasNumberInOperation){
						i = 0;
						HasNumberInOperation = false;
						continue;
					}
					FindDualOperands(i);
					char Result = PerformSubtraction();
					std::string Result_s {Result};
					UpdateFunction(LeftDualOperandPos, RightDualOperandPos, Result_s);
					i = 0;
				}
			}
			
			char FinalUsedVariable = CurrentUnusedVariable - 1;
			int FinalDualNumberIndex = FinalUsedVariable - 97;
			if(InParentheticFunction == false)
				std::cout << DualVector.at(FinalDualNumberIndex).Value << "+" << DualVector.at(FinalDualNumberIndex).Derivative << "D" << std::endl;
        }
		
		void FindDualOperands(int OperatorPosition){
			for(int j = OperatorPosition; j >= 0; j--){
				if(int(Function.at(j)) >= 97 && int(Function.at(j)) <= 122){
					LeftDualOperand = Function.at(j);
					LeftDualOperandPos = j;
					break;
				}
			}
				
				
			for(int j = OperatorPosition; j < static_cast<int>(Function.size()); j++)
				if(int(Function.at(j)) >= 97 && int(Function.at(j)) <= 122){
					RightDualOperand = Function.at(j);
					RightDualOperandPos = j;
					break;
				}
		}
		
		void UpdateFunction(int StartPos, int EndPos, std::string Result){
			Function.replace(StartPos, (EndPos - StartPos) + 1, Result); 
		}
		
		char PerformMultiplication(){
			Dual MultipliedResult;
			int LeftDualNumberIndex = int(LeftDualOperand) - 97;
			int RightDualNumberIndex = int(RightDualOperand) - 97;
			MultipliedResult.Value = DualVector.at(LeftDualNumberIndex).Value * DualVector.at(RightDualNumberIndex).Value;
			MultipliedResult.Derivative = DualVector.at(LeftDualNumberIndex).Value * DualVector.at(RightDualNumberIndex).Derivative + DualVector.at(LeftDualNumberIndex).Derivative * DualVector.at(RightDualNumberIndex).Value;
			DualVector.push_back(MultipliedResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		char PerformDivision(){
			Dual DividedResult;
			int LeftDualNumberIndex = int(LeftDualOperand) - 97;
			int RightDualNumberIndex = int(RightDualOperand) - 97;
			DividedResult.Value = DualVector.at(LeftDualNumberIndex).Value / DualVector.at(RightDualNumberIndex).Value;
			DividedResult.Derivative = (DualVector.at(LeftDualNumberIndex).Derivative * DualVector.at(RightDualNumberIndex).Value 
			- DualVector.at(LeftDualNumberIndex).Value * DualVector.at(RightDualNumberIndex).Derivative) / std::pow(DualVector.at(RightDualNumberIndex).Value, 2);
			DualVector.push_back(DividedResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		char PerformAddition(){
			Dual AddedResult;
			int LeftDualNumberIndex = int(LeftDualOperand) - 97;
			int RightDualNumberIndex = int(RightDualOperand) - 97;
			AddedResult.Value = DualVector.at(LeftDualNumberIndex).Value + DualVector.at(RightDualNumberIndex).Value;
			AddedResult.Derivative = DualVector.at(LeftDualNumberIndex).Derivative + DualVector.at(RightDualNumberIndex).Value;
			DualVector.push_back(AddedResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		char PerformSubtraction(){
			Dual SubtractedResult;
			int LeftDualNumberIndex = int(LeftDualOperand) - 97;
			int RightDualNumberIndex = int(RightDualOperand) - 97;
			SubtractedResult.Value = DualVector.at(LeftDualNumberIndex).Value - DualVector.at(RightDualNumberIndex).Value;
			SubtractedResult.Derivative = DualVector.at(LeftDualNumberIndex).Derivative - DualVector.at(RightDualNumberIndex).Value;
			DualVector.push_back(SubtractedResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		char EvaluateExponent(int ExpOperatorIndex){
			char DualOperand = Function.at(ExpOperatorIndex - 1);
			int DualNumberIndex = int(DualOperand) - 97;
			for(int i = ExpOperatorIndex + 1; i < static_cast<int>(Function.size()); i++){
				char NextOperator = Function.at(i);
				if((NextOperator == '+' || NextOperator == '-' || NextOperator == '*' || NextOperator == '/') && !(isdigit(Function.at(i+1)))){
					NextOperatorIndex = i;
				}
			}
			std::string Power_s = Function.substr(ExpOperatorIndex + 1, NextOperatorIndex - (ExpOperatorIndex + 1));
			long double Power = std::stold(Power_s);
			Dual PowerResult;
			PowerResult.Value = std::pow(DualVector.at(DualNumberIndex).Value, Power);
			PowerResult.Derivative = Power * std::pow(DualVector.at(DualNumberIndex).Value, (Power - 1)) * DualVector.at(DualNumberIndex).Derivative;
			DualVector.push_back(PowerResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		char EvaluateParentheses(int LeftParenthesesIndex){
			LeftParenthesesPos = LeftParenthesesIndex;
			for(int i = LeftDualOperandPos + 1; i < static_cast<int>(Function.size()); i++){
				if(Function.at(i) == ')'){
					RightParenthesesPos = i;
					break;
				}
			}
			ParentheticFunction = Function.substr(LeftParenthesesPos + 1, RightParenthesesPos - LeftParenthesesPos - 1);
			EvaluateFunctions ParentheticEvaluator(ParentheticFunction, DualVector);
			ParentheticEvaluator.InParentheticFunction = true;
			ParentheticEvaluator.Evaluate();
			int FunctionDualNumberIndex = (ParentheticEvaluator.CurrentUnusedVariable - 1) - 97;
			Dual ParentheticResult = ParentheticEvaluator.DualVector.at(FunctionDualNumberIndex);
			DualVector.push_back(ParentheticResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		char EvaluateSin(int MathFunctionIndex){
			char InputDualOperand {Function.at(MathFunctionIndex + 3)};
			int InputDualNumberIndex {int(InputDualOperand) - 97};
			Dual SinResult;
			SinResult.Value = std::sin(DualVector.at(InputDualNumberIndex).Value);
			SinResult.Derivative = std::cos(DualVector.at(InputDualNumberIndex).Value);
			DualVector.push_back(SinResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		char EvaluateCos(int MathFunctionIndex){
			char InputDualOperand {Function.at(MathFunctionIndex + 3)};
			int InputDualNumberIndex {int(InputDualOperand) - 97};
			Dual CosResult;
			CosResult.Value = std::cos(DualVector.at(InputDualNumberIndex).Value);
			CosResult.Derivative = -1 * std::sin(DualVector.at(InputDualNumberIndex).Value);
			DualVector.push_back(CosResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		char EvaluateTan(int MathFunctionIndex){
			char InputDualOperand {Function.at(MathFunctionIndex + 3)};
			int InputDualNumberIndex {int(InputDualOperand) - 97};
			Dual TanResult;
			TanResult.Value = std::tan(DualVector.at(InputDualNumberIndex).Value);
			TanResult.Derivative = std::pow(1 / (std::cos(DualVector.at(InputDualNumberIndex).Value)), 2);
			DualVector.push_back(TanResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		char EvaluateCot(int MathFunctionIndex){
			char InputDualOperand {Function.at(MathFunctionIndex + 3)};
			int InputDualNumberIndex {int(InputDualOperand) - 97};
			Dual CotResult;
			CotResult.Value = (1 / std::tan(DualVector.at(InputDualNumberIndex).Value));
			CotResult.Derivative = std::pow(-1 / (std::sin(DualVector.at(InputDualNumberIndex).Value)), 2);
			DualVector.push_back(CotResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		char EvaluateCosec(int MathFunctionIndex){
			char InputDualOperand {Function.at(MathFunctionIndex + 3)};
			int InputDualNumberIndex {int(InputDualOperand) - 97};
			Dual CscResult;
			CscResult.Value = 1 / std::sin(DualVector.at(InputDualNumberIndex).Value);
			CscResult.Derivative = (-1 / std::sin(DualVector.at(InputDualNumberIndex).Value)) * (1 / std::tan(DualVector.at(InputDualNumberIndex).Value));
			DualVector.push_back(CscResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		char EvaluateSec(int MathFunctionIndex){
			char InputDualOperand {Function.at(MathFunctionIndex + 3)};
			int InputDualNumberIndex {int(InputDualOperand) - 97};
			Dual SecResult;
			SecResult.Value = 1 / std::cos(DualVector.at(InputDualNumberIndex).Value);
			SecResult.Derivative = (1 / std::cos(DualVector.at(InputDualNumberIndex).Value)) * (std::tan(DualVector.at(InputDualNumberIndex).Value));
			DualVector.push_back(SecResult);
			char Result = CurrentUnusedVariable;
			CurrentUnusedVariable++;
			return Result;
		}
		
		void DualArithmeticWithNumberEvaluator(int OperatorIndex){
			std::string LeftNumberOrDual_s {};
			std::string RightNumberOrDual_s {};
			int LeftNumberOrDualPos {};
			int RightNumberOrDualPos {};
			
			for(int i = OperatorIndex - 1; i >= 0; i--){
				if(i == 0 && i == '-'){
					LeftNumberOrDualPos = i;
					break;
				}
				else if(Function.at(i) == '+' || Function.at(i) == '-' || Function.at(i) == '*' || Function.at(i) == '/'){
					LeftNumberOrDualPos = i + 1;
					break;
				}
				else if(i == 0){
					LeftNumberOrDualPos = i;
					break;
				}
			}
			LeftNumberOrDual_s = Function.substr(LeftNumberOrDualPos, OperatorIndex - LeftNumberOrDualPos);
			
			for(int i = OperatorIndex + 1; i < static_cast<int>(Function.size()); i++){
				if(Function.at(i) == '+' || Function.at(i) == '-' || Function.at(i) == '*' || Function.at(i) == '/'){
					RightNumberOrDualPos = i - 1;
					break;
				}
				else if(i == static_cast<int>(Function.size()) - 1){
					RightNumberOrDualPos = i;
					break;
				}
			}
			RightNumberOrDual_s = Function.substr(OperatorIndex + 1, RightNumberOrDualPos - OperatorIndex);
			
			long double LeftNumber {};
			long double RightNumber {};
			
			if(IsNumber(LeftNumberOrDual_s)){
				LeftNumber = std::stold(LeftNumberOrDual_s);
				HasNumberInOperation = true;
			}
			else{
				if(LeftNumberOrDual_s.at(0) == '-' || LeftNumberOrDual_s.at(0) == '+'){
					LeftNumberOrDual_s.erase(0);
					LeftNumberOrDualPos++;
				}
			}
			
			if(IsNumber(RightNumberOrDual_s)){
				RightNumber = std::stold(RightNumberOrDual_s);
				HasNumberInOperation = true;
			}
			
			char Result {};
			if(HasNumberInOperation){
				if(LeftNumber){
					char DualOperand = RightNumberOrDual_s.at(0);
					int DualNumberIndex = int(DualOperand) - 97;
					Dual NumericArithmeticResult;
					if(Function.at(OperatorIndex) == '*'){
						NumericArithmeticResult.Value = DualVector.at(DualNumberIndex).Value * LeftNumber;
						NumericArithmeticResult.Derivative = DualVector.at(DualNumberIndex).Derivative * LeftNumber;
					}
					else if(Function.at(OperatorIndex) == '/'){
						NumericArithmeticResult.Value = DualVector.at(DualNumberIndex).Value / LeftNumber;
						NumericArithmeticResult.Derivative = DualVector.at(DualNumberIndex).Derivative / LeftNumber;
					}
					else if(Function.at(OperatorIndex) == '+'){
						NumericArithmeticResult.Value = DualVector.at(DualNumberIndex).Value + LeftNumber;
						NumericArithmeticResult.Derivative = DualVector.at(DualNumberIndex).Derivative;
					}
					else if(Function.at(OperatorIndex) == '-'){
						NumericArithmeticResult.Value = DualVector.at(DualNumberIndex).Value - LeftNumber;
						NumericArithmeticResult.Derivative = DualVector.at(DualNumberIndex).Derivative;
					}
					DualVector.push_back(NumericArithmeticResult);
					Result = CurrentUnusedVariable;
					CurrentUnusedVariable++;
				}
				
				if(RightNumber){
					char DualOperand = LeftNumberOrDual_s.at(0);
					int DualNumberIndex = int(DualOperand) - 97;
					Dual NumericArithmeticResult;
					if(Function.at(OperatorIndex) == '*'){
						NumericArithmeticResult.Value = DualVector.at(DualNumberIndex).Value * RightNumber;
						NumericArithmeticResult.Derivative = DualVector.at(DualNumberIndex).Derivative * RightNumber;
					}
					else if(Function.at(OperatorIndex) == '/'){
						NumericArithmeticResult.Value = DualVector.at(DualNumberIndex).Value / RightNumber;
						NumericArithmeticResult.Derivative = DualVector.at(DualNumberIndex).Derivative / RightNumber;
					}
					else if(Function.at(OperatorIndex) == '+'){
						NumericArithmeticResult.Value = DualVector.at(DualNumberIndex).Value + RightNumber;
						NumericArithmeticResult.Derivative = DualVector.at(DualNumberIndex).Derivative;
					}
					else if(Function.at(OperatorIndex) == '-'){
						NumericArithmeticResult.Value = DualVector.at(DualNumberIndex).Value - RightNumber;
						NumericArithmeticResult.Derivative = DualVector.at(DualNumberIndex).Derivative;
					}
					DualVector.push_back(NumericArithmeticResult);
					Result = CurrentUnusedVariable;
					CurrentUnusedVariable++;
				}
				std::string Result_s {Result};
				UpdateFunction(LeftNumberOrDualPos, RightNumberOrDualPos, Result_s);
			}
		}
};

int main(){
    size_t NumberOfDualNumbers {};
    std::cout << "Type the number of dual numbers in your function: ";
    std::cin >> NumberOfDualNumbers;
    
    std::vector<Dual> DualVector = {};
    
    for(int i = 0; i < static_cast<int>(NumberOfDualNumbers); i++){
        Dual ithDualNumber;
        long double Value {}, Derivative {};
        std::cout << "Input the value and the derivative seperated by space of the " << i + 1 << "th dual number: ";
        std::cin >> Value >> Derivative;
        ithDualNumber.AssignDualNumber(Value, Derivative);
        DualVector.push_back(ithDualNumber);
    }

    EvaluateFunctions Evaluator(DualVector);
    Evaluator.InputFunction();
	std::cout << "RESULT:" << std::endl;
    Evaluator.Evaluate();

    return 0;
}