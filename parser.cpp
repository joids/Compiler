#include "parser.h"

AST* parser::pars(linkToken* Token){
	Token = Token;
	count = 0;
	tab = 0;
	lookahead = Token;
	root = NULL;
	start();
	printf("End parsing\n");
	return root;

}

void parser::start(){
	AST* StartNode = initASTNode();
	setLine(StartNode, "start");
	root = StartNode;
	
	StList(StartNode);
}

void parser::StList(AST* StartNode){
	 if (lookahead->type == "print"|| 
        lookahead->type == "if"||
        lookahead->type == "while"||
        lookahead->type == "id"||
        lookahead->type == "def"||
        lookahead->type == "return")
    {
        Anuthing(StartNode);
        StList(StartNode);
    }else
        if (lookahead->type == "comma" ||
		lookahead->type == "less_or_equal" || 
        lookahead->type == "more_or_equal" || 
        lookahead->type == "not_equal"||
        lookahead->type == "less" ||
        lookahead->type == "more" ||
        lookahead->type == "l_paren" ||
        lookahead->type == "r_paren" ||
        lookahead->type == "l_braket" ||
        lookahead->type == "r_braket" ||
        lookahead->type == "equally" ||
        lookahead->type == "semicolon")
    {
		printErrorMessage(lookahead->y,lookahead->x, "Statement list");
        exit(1);
    }

}
void parser::Anuthing(AST* StartNode){
	if (lookahead->type == "print") {
		AST* printNode = initASTNode();
		setLine(printNode, "print");
		add_child(printNode, StartNode);
		
		print(printNode);
	}else if (lookahead->type =="id"){
		Tat(StartNode);
	} else if (lookahead->type == "if"){
		If(StartNode);
	} else if (lookahead->type == "while"){
		While(StartNode);
	} else if (lookahead->type == "return"){
		Return(StartNode);
	} else if (lookahead->type == "def"){
		func(StartNode);
	}
}
void parser::print(AST *node) {
	match("print");
	match("l_paren");
	printarg(node);
	match("r_paren");
}
void parser::printarg(struct AST *node) {
	if (lookahead->type == "id"){
		commaid(node);
	} else if (lookahead->type == "literal"){
		commaid(node);
	} else {
		printErrorMessage(lookahead->y,lookahead->x, "id or literal");
		exit(1);
	}  
}
void parser::commaid(AST* node){
	headcommaid(node);
	tailcommaid(node);
}

void parser::headcommaid(AST* node){
	if (lookahead->type == "id"){
		AST* idNode = initASTNode();
		setLine(idNode, "var");
		setToken(idNode, getLookahead());
	
		add_child(idNode, node);
		match("id");
	}
	if (lookahead->type == "literal"){
		AST* literalNode = initASTNode();
		setLine(literalNode, "literal");
		setToken(literalNode, getLookahead());

		add_child(literalNode, node);
		match("literal");
	}
}

void parser::tailcommaid(AST* node) {
	if (lookahead->type == "comma"){
		match("comma");
		if (lookahead->type == "id"){
			AST* idNode = initASTNode();
			setLine(idNode, "var");
			setToken(idNode, getLookahead());
	
			add_child(idNode, node);
			match("id");
			tailcommaid(node);
		}else if (lookahead->type == "literal"){
			AST* literalNode = initASTNode();
			setLine(literalNode, "literal");
			setToken(literalNode, getLookahead());

			add_child(literalNode, node);
			match("literal");
			tailcommaid(node);
		}else{
			printErrorMessage(lookahead->y,lookahead->x, "id or literal");
			exit(1);		
		}
	}
}
void parser::If(struct AST* StatementNode)
{
    struct AST* IfNode = initASTNode();
    setLine(IfNode, "if");
    add_child(IfNode, StatementNode);

    match("if");

    Expr(IfNode);
    
    match("colon");
    match("l_brace");

    struct AST* StatNode = initASTNode();
    setLine(StatNode, "StatementList");
    add_child(StatNode, IfNode);

    StList(StatNode); 
    match("r_brace");
    Else(IfNode);
}
/*
<else> -> else <else`> | E
*/
void parser::Else(struct AST* IfNode)
{
    if (lookahead->type == "else")
    {
        match("else");
        Else_T(IfNode);
    }
	if (lookahead->type == "elif")
    {
        match("elif");
        Elif_T(IfNode);
    }
}
/*
<else_t> -> { <statementList> } | <if> 
*/
void parser::Else_T(struct AST* IfNode)
{
    if (lookahead->type == "colon")
    {
        match("colon");
		match("l_brace");

        struct AST* StatNode = initASTNode();
        setLine(StatNode, "StatementList");
        add_child(StatNode, IfNode);

        StList(StatNode);

	    match("r_brace");
    	Else(IfNode);	
    } 
}
void parser::Elif_T(struct AST* IfNode)
{
    Expr(IfNode);
    
    match("colon");
    match("l_brace");

    struct AST* StatNode = initASTNode();
    setLine(StatNode, "StatementList");
    add_child(StatNode, IfNode);

    StList(StatNode); 
    match("r_brace");
    Else(IfNode);
}
/*
<while> -> while (<expr>) { <statementList> }
*/
void parser::While(struct AST* StatementNode)
{
    struct AST* WhileNode = initASTNode();
    setLine(WhileNode, "while");
    add_child(WhileNode, StatementNode);
    
    match("while");

    Expr(WhileNode);

    match("colon");
    match("l_brace");

    struct AST* StatNode = initASTNode();
    setLine(StatNode, "StatementList");
    add_child(StatNode, WhileNode);

    StList(StatNode);

    match("r_brace");
}
/*
<expr> -> numeric | id <compar>
*/
void parser::Expr(struct AST* Node)
{
    struct AST* ExprNode = initASTNode();
    setLine(ExprNode, "Expresion");
    add_child(ExprNode, Node);

    if (lookahead->type == "numeric")
    {
        struct AST* NumericNode = initASTNode();
        setLine(NumericNode, "numeric");
        setToken(NumericNode,  getLookahead());
        add_child(NumericNode, ExprNode);

        match("numeric");
    }else if (lookahead->type == "id")
    {
        struct AST* IdNode = initASTNode();
        setLine(IdNode, "var");
        setToken(IdNode,  getLookahead());
        add_child(IdNode, ExprNode);
        

        match("id");
        Compar(ExprNode);
    }else{
		printErrorMessage(lookahead->y,lookahead->x, "Expresion");
        exit(1);
    }
}
/*
<copmar> -> <comparison> <id_or_num> | E
*/
void parser::Compar(struct AST* ExprNode){
    if (lookahead->type =="less_or_equal" || 
        lookahead->type =="no_compare" ||
        lookahead->type =="less" ||
        lookahead->type =="more" || 
        lookahead->type =="more_or_equal")
    {
        struct AST* ComparNode = initASTNode();
        setLine(ComparNode, lookahead->type);
        swapChild(ExprNode, ComparNode);
        //add_child(ComparNode, ExprNode);

        Comparison(ExprNode);
       	Id_or_Num(ComparNode);
    }
}
/*
<comparison> -> == | != | < | <= | > | >=
*/
void parser::Comparison(struct AST* ExprNode)
{
    //struct AST* ComparNode = Init_Node_AST();
    if(lookahead->type =="less_or_equal")
    {
        match("less_or_equal");
    } else if (lookahead->type =="no_compare")
    {
        match("no_compare");
    } else if (lookahead->type =="less") 
    {
        match("less");
    } else if (lookahead->type =="more")
    {
        match("more");
    } else if (lookahead->type =="more_or_equal")
    {
        match("more_or_equal");
    }else{
		printErrorMessage(lookahead->y,lookahead->x, "comparison");
    	exit(1);
    }
}
/*
<or_and> -> || | && 
*/
void parser::Or_And()
{
    if (lookahead->type == "or")
    {
        match("or");
    }else if (lookahead->type == "and")
    {
        match("and");
    }else{
		printErrorMessage(lookahead->y,lookahead->x, "or_and");
        exit(1);
    }
}

void parser::Id_or_Num(struct AST* ComparNode)
{
    if (lookahead->type == "id")
    {
        struct AST* IdNode = initASTNode();
        setLine(IdNode, "var");
        setToken(IdNode,  getLookahead());
       	add_child(IdNode, ComparNode);
		//IdNode->type = 1;


        match("id");
    } else if (lookahead->type == "minus" ||
                lookahead->type == "plus" ||
                lookahead->type == "numeric")
    {
        Neg_Sings(ComparNode);

        if (lookahead->type != "numeric"){
			printErrorMessage(lookahead->y,lookahead->x, "numeric");
            exit(1);
        }
        struct AST* NumericNode = initASTNode();
        setLine(NumericNode, "numeric");
        setToken(NumericNode, getLookahead());
        //Set_Token(IdNode,  parser->knots);
        add_child(NumericNode, ComparNode);

        match("numeric");
    }else if (lookahead->type ==  "input"){
		input1(ComparNode);
	}else{
        if (lookahead->type != "minus" ||
            lookahead->type != "plus" ||
            lookahead->type != "numeric" ||
            lookahead->type != "id") 
        {
			printErrorMessage(lookahead->y,lookahead->x, "calc");		
            exit(1);
        }
    }
}

void parser::Neg_Sings(AST* node)
{
    if (lookahead->type == "plus")
    {
        match("plus");
    }else if (lookahead->type == "minus")
    {
        match("minus");
    }
}

/*
<equal> -> numeric | literal   
*/
void parser::Equal(struct AST* Node)
{
    if (lookahead->type ==  "numeric")
    {
        struct AST* NumericNode = initASTNode();
        setLine(NumericNode, "numeric");
        setToken(NumericNode, getLookahead());
        add_child(NumericNode, Node);

        match("numeric");
    }else if (lookahead->type == "literal")
    {
        struct AST* LiteralNode = initASTNode();
        setLine(LiteralNode, "Literal");
        setToken(LiteralNode,  getLookahead());
        add_child(LiteralNode, Node);

       match("literal");
    }else{
		printErrorMessage(lookahead->y,lookahead->x, "numeric or literal");	
        exit(1);
    }
}
/*
<tat> -> id <arith_or_func>
*/
void parser::Tat(struct AST* StatementNode)
{

    struct AST* IdNode = initASTNode();
    add_child(IdNode, StatementNode);
	IdNode->type = 1;
	setToken(IdNode,  getLookahead());
    match("id");
    Arith_or_func(StatementNode);
}
/*
<arith_or_func> -> <func_call> | <arithmetic>
*/
void parser::Arith_or_func(struct AST* StatementNode){
    if (lookahead->type == "l_paren"){
        struct AST* getLastNode = getLastChilde(StatementNode);;
        setLine(getLastNode, "Function");

        struct AST* FuncCallNode = initASTNode();
        setLine(FuncCallNode, "Func_call");
        swapChild(StatementNode, FuncCallNode);
       //Add_Child(FuncCallNode, StatementNode);

        Func_call(FuncCallNode);
    }else if (lookahead->type ==  "l_braket"){
		struct AST* getLastNode = getLastChilde(StatementNode);;
	    setLine(getLastNode, "Array");
		getLastNode->type = 3;
	    array(getLastNode);
    }else if (lookahead->type ==  "equally"){
		struct AST* getLastNode = getLastChilde(StatementNode);
		setLine(getLastNode, "var");
	   // Set_Token(getLastNode, Get_knots);
	    struct AST* ArithmeticNode = initASTNode();
	    setLine(ArithmeticNode, "Arithmetic");
	    swapChild(StatementNode, ArithmeticNode);
	    Arithmetic(ArithmeticNode);
    }else{
		printErrorMessage(lookahead->y,lookahead->x, "function or Arithmetic");
        exit(1);
    }
}
void parser::array(struct AST* Node){
	match("l_braket");
	if (lookahead->type ==  "id"){
	    struct AST* IdNode = initASTNode();
		setLine(IdNode, "arr_index");
	    add_child(IdNode, Node);
		match("id");
		match("r_braket");  
		if (lookahead->type ==  "equally"){
			Arithmetic(Node);
		}  
	}else if (lookahead->type ==  "numeric"){
		struct AST* IdNode = initASTNode();	
		setLine(IdNode, "arr_index");
	    add_child(IdNode, Node);
		match("numeric");
		match("r_braket");  
		if (lookahead->type ==  "equally"){
			Arithmetic(Node);
		}     
	}else{
		printErrorMessage(lookahead->y,lookahead->x, "id or numeric");
        exit(1);
    }
}
/*
<func_call> ->  (id , id);
*/
void parser::Func_call(struct AST* IdNode )
{
    match("l_paren");
    if (lookahead->type !=  "id")
    {
		printErrorMessage(lookahead->y,lookahead->x, "id");
        exit(1);
    }
    struct AST* ArgListNode = initASTNode();
    setLine(ArgListNode, "Arg_List");
    add_child(ArgListNode, IdNode);
    
    struct AST* Id__Node = initASTNode();
    setLine(Id__Node, "var");
    setToken(IdNode,  getLookahead());
    add_child(Id__Node, ArgListNode);


    match("id");
    if (lookahead->type != "comma"){
		printErrorMessage(lookahead->y,lookahead->x, "comma");
        exit(1);
    }
    match("comma");

    if (lookahead->type !="id"){
		printErrorMessage(lookahead->y,lookahead->x, "id");
        exit(1);
    }

    struct AST* Id_Node = initASTNode();
    setLine(Id_Node, "var");
    setToken(IdNode, getLookahead());
    add_child(Id_Node, ArgListNode);


    match("id");
    if (lookahead->type != "r_paren"){
		printErrorMessage(lookahead->y,lookahead->x, "id");
        exit(1);
    }
    match("r_paren");
}
/*
<arithmetic> ->  = <oror>;
*/
void parser::Arithmetic(struct AST* ArithmeticNode)
{
    struct AST* ComparNode = initASTNode();
    setLine(ComparNode, getLookaheadname());
	setToken(ComparNode, getLookahead());
    swapChild(ArithmeticNode, ComparNode);

    match("equally");
    Oror(ComparNode);
}
/*
<oror> -> <vot> 
*/
void parser::Oror(struct AST* EquallyNode)
{
    Vot(EquallyNode);
}
/*
<vot> -> <id_or_num> <top>
*/
void parser::Vot(struct AST* Node)
{
    Id_or_Num(Node);
    Top(Node);
}
/*
<top> -> <mult_or_add> <id_or_num> | E 
*/
void parser::Top(struct AST* EquallyNode)
{
    if (lookahead->type == "star" ||
        lookahead->type == "division" ||
        lookahead->type == "plus" || 
        lookahead->type == "minus")
    {
        struct AST* ComparNode = initASTNode();
        setLine(ComparNode, getLookaheadname());
        swapChild(EquallyNode, ComparNode);

        Mult_or_Add(EquallyNode);
        Id_or_Num(ComparNode);
    }
}

/*
<mult_Oper> -> * | /
*/
void parser::Mult_Oper(struct AST* AnnouncementNode)
{
    if (lookahead->type == "star")
    {
        struct AST* StarNode = initASTNode();
        setLine(StarNode, "star");
        add_child(StarNode, AnnouncementNode);

        match("star");
    }else if (lookahead->type == "division")
    {
        struct AST* DivisionNode = initASTNode();
        setLine(DivisionNode, "division");
        add_child(DivisionNode, AnnouncementNode);

        match("division");
    }else{
		printErrorMessage(lookahead->y,lookahead->x, "mult_oper");
        exit(1);
    }
    
}
/*
<add_Oper> -> + | -
*/
void parser::Add_Oper(struct AST* AnnouncementNode)
{
    if (lookahead->type == "plus")
    {
        match("plus");
    }else if (lookahead->type == "minus")
    {
        match("minus");
    }else{
		printErrorMessage(lookahead->y,lookahead->x, "plus or minus");
        exit(1);
    }
}
/*
<mult_or_add> -> <mult_Oper> | <add_Oper>
*/
void parser::Mult_or_Add(struct AST* AnnouncementNode)
{
    if (lookahead->type == "plus" ||
        lookahead->type == "minus") 
    {
        Add_Oper(AnnouncementNode);
    }else if (lookahead->type == "division" ||
                lookahead->type == "star")
    {
        Mult_Oper(AnnouncementNode);
    }else if (lookahead->type != "division" ||
            lookahead->type != "star" ||
            lookahead->type != "plus" ||
            lookahead->type != "minus")
        {
			printErrorMessage(lookahead->y,lookahead->x, "plus or minus");
            exit(1);
        }
}
/*
<return> -> return <return_Value>;
*/
void parser::Return(struct AST* StatementNode)
{
    struct AST* ReturnNode = initASTNode();
    setLine(ReturnNode, "Return");
    add_child(ReturnNode, StatementNode);

    match("return");
    Return_Value(ReturnNode);
}
/*
<return_Value> -> <neg_sings>numeric | E
*/
void parser::Return_Value(struct AST* ReturnNode)
{
    if (lookahead->type ==  "minus" ||
                lookahead->type ==  "plus" ||
				lookahead->type ==  "id" ||
                lookahead->type == "numeric")
    {
        Neg_Sings(ReturnNode);
        if (lookahead->type == "numeric")
        {
		    struct AST* NumericNode = initASTNode();
		    setLine(NumericNode, "numeric");
		    add_child(NumericNode, ReturnNode);
			match("numeric");
        }
		if (lookahead->type == "id")
        {
		    struct AST* NumericNode = initASTNode();
		    setLine(NumericNode, "var");
		    add_child(NumericNode, ReturnNode);
			match("id");
        }
    
    
    }
}






void parser::input1(AST* node) {
	AST* inputNode = initASTNode();
	setLine(inputNode, "input");
	add_child(inputNode, node);
	match("input");
    input_l_par(node);
}

void parser::input_l_par(AST* node) {
	if (lookahead->type == "l_paren"){
		match("l_paren");
        input_var(node);
	}else{
		printErrorMessage(lookahead->y,lookahead->x, "l_paren");
		exit(1);
	}   
}

void parser::input_var(AST* node) {
	if (lookahead->type == "literal"){
		AST* literalNode = initASTNode();
		setLine(literalNode, "literal");
		add_child(literalNode, node);

		match("literal");
        input_r_par(literalNode);
	} else 	if(lookahead->type == "r_paren"){
		match("r_paren");
	}else{
		printErrorMessage(lookahead->y,lookahead->x, "r_paren or literal");
		exit(1);
	}
}
void parser::input_r_par(AST* node) {
	if (lookahead->type == "r_paren"){
		match("r_paren");
	}else{
		printErrorMessage(lookahead->y,lookahead->x, "r_paren");
		exit(1);
	}   
}

void parser::func(AST* StartNode) {
	struct AST* FuncNode = initASTNode();
	setLine(FuncNode, "def");
	add_child(FuncNode, StartNode);

	match("def");
	match("id");
	match("l_paren");
	struct AST *argListNode = initASTNode();
	setLine(argListNode, "Arg_List");
	add_child(argListNode, FuncNode);

	commaid(argListNode);
	match("r_paren");
	match("colon");
	match("l_brace");
	
	struct AST *bodyFuncNode = initASTNode();
	setLine(bodyFuncNode, "bodyFunc");
	add_child(bodyFuncNode, FuncNode);

	StList(bodyFuncNode);

    match("r_brace");
	
}

void parser::consume(){
	lookahead = nextToken(); 
}

linkToken* parser::nextToken(){
	count++;
	return lookahead->next;
}

linkToken* parser::getLookahead(){
	return lookahead;
}
string parser::getLookaheadname(){
	return lookahead->type;
}
void parser::match(string x) {
	if(lookahead->type == x){
		consume();
	} else {
		printErrorMessage(lookahead->y,lookahead->x, x);
		exit(1);
	}
}

void parser::printErrorMessage(int row, int column, string x) {
	cout << row << ":" << column << " error: expected: " << x << " find " << endl;
}
