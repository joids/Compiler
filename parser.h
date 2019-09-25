#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "listToken.h"
#include "lexer.h"
#include "AST.h"

using namespace std;

class parser{
	private:
		linkToken* Token;
		linkToken* lookahead;
		int count;
		int tab;
		struct AST *root;
	public:
		AST* pars(linkToken* Token);
		void start();
		void StList(AST* StartNode);
		void Anuthing(AST* StartNode);
		void print(AST *node);
		void Lit_or_id(struct AST* LiteralNode);
		void If(struct AST* StatementNode);
		void Else(struct AST* IfNode);
		void Else_T(struct AST* IfNode);
		void Elif_T(struct AST* IfNode);
		void While(struct AST* StatementNode);
		void Expr(struct AST* Node);
		void Compar(struct AST* ExprNode);
		void Comparison(struct AST* ExprNode);
		void Or_And();
		void Equal(struct AST* Node);
		void Tat(struct AST* StatementNode);
		void Arith_or_func(struct AST* StatementNode);
		void Func_call(struct AST* IdNode );
		void Arithmetic(struct AST* ArithmeticNode);
		void Oror(struct AST* EquallyNode);
		void Vot(struct AST* Node);
		void Top(struct AST* EquallyNode);
		void Mult_Oper(struct AST* AnnouncementNode);
		void Add_Oper(struct AST* AnnouncementNode);
		void Mult_or_Add(struct AST* AnnouncementNode);
		void Id_or_Num(struct AST* ComparNode);
		void Return(struct AST* StatementNode);
		void Return_Value(struct AST* ReturnNode);
		void array(struct AST* Node);
		void Neg_Sings(AST* node);
		void printarg(AST *node);
		void commaid(AST* node);
		void headcommaid(AST* node);
		void tailcommaid(AST* node);
		void input1(AST* node);
		void input_l_par(AST* node);
		void input_var(AST* node);
		void input_r_par(AST* node);
		void id1(AST* node);
		void id2(AST* node);
		void idequally(AST* node);
		void idfunc(AST* node);
		void arg(AST* node);
		void comma_arg(AST* node);
		void headcomma_arg(AST* node);
		void tailcomma_arg(AST* node);
		//void idmas(AST* node);
		void arithmetic(AST* node);
		void headarithmetic(AST* node);
		void tailarithmetic(AST* node);
		void func(AST* node);

		linkToken* nextToken();
		linkToken* getLookahead();
		string getLookaheadname();
		void consume();
		void printErrorMessage(int row, int column, string x);
		void match(string x);
};


#endif
