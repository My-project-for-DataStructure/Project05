#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>



#define MAX 10 

typedef struct ano *nodeptr;
typedef struct ano {
	char name[30];
	float gpa;
	char place[100];
	nodeptr links[MAX];
}nodetype;

nodeptr ROOT;
nodeptr stack[100];
int top = -1;
int check = 1;

int print_nm(nodeptr Root, char *sname);
void print_nm2(nodeptr curr);
int print_bro(nodeptr cur, char *sname);
void print_tree(nodeptr curr);
void p_bro(nodeptr curr);
nodeptr search_node(nodeptr cur, char sname[]) {
	int i;
	nodeptr rptr;
	if (!cur)
		return NULL;

	if (strcmp(cur->name, sname) == 0) {
		return cur;
	}
	else {
		for (i = 0; cur->links[i] != NULL; i++) {
			rptr = search_node(cur->links[i], sname);
			if (rptr)
				return rptr;
		}
		return NULL;
	}

}

int read_upper(FILE *fp, char buf[20][50]) {
	int i, j, k;
	char line[400];
	char *cp;
	cp = fgets(line, 400, fp);

	if (!cp) {
		printf("Imppssible for the control to reach here.\n");
		return 0;
	}
	else {
		if (line[0] == '-')
			return 0;

		i = 0;
		j = 0;
		while (1) {
			k = 0;
			while (line[j] != ' ' && line[j] != '\n') {
				buf[i][k] = line[j];
				j++;
				k++;
			}
			buf[i][k] = '\0';
			i++;
			if (line[j] == '\n')
				break;
			else {
				do j++; while (line[j] == ' ');
			}
		}
		return i;
	}
}

void read_store_lower_data(FILE *fp) {
	char rname[30], address[100];
	float rgpa;
	nodeptr np;
	int read_num;

	do {
		read_num = fscanf(fp, "%s%f%s", rname, &rgpa, address);
		if (read_num != 3)
			break;
		else {
			np = search_node(ROOT, rname);
			if (!np)
				printf("이런 이름을 가진 노드가 없습니다.\n");
			else
				strcpy(np->name, rname); np->gpa = rgpa; strcpy(np->place, address);
		}
	} while (1);
}

void print_general_tree(nodeptr curr) {
	int i;
	if (!curr)
		return;
	printf("이름:%s   학점:%5.2f   주소:%s \n", curr->name, curr->gpa, curr->place);
	for (i = 0; curr->links[i]; i++)
		print_general_tree(curr->links[i]);
}

void make_node_and_attach_to_parent(nodeptr parent, char *tname, int loc) {
	nodeptr np1;
	np1 = (nodeptr)malloc(sizeof(nodetype));
	strcpy(np1->name, tname);
	np1->links[0] = NULL;
	parent->links[loc] = np1;
}

void dfs_height(nodeptr cur, int par_height, int *tree_height) {
	int my_height = par_height + 1;
	int i;
	if (*tree_height < my_height)
		*tree_height = my_height;
	for (i = 0; cur->links[i]; i++)
		dfs_height(cur->links[i], my_height, tree_height);
	return;
}

int dfs_depth(nodeptr cur, int par_height, char *sname) {
	int my_height = par_height + 1;
	int i, res;
	if (strcmp(cur->name, sname) == 0) {
		printf("height of the node of %s : %d\n", sname, my_height);
		return 1;
	}
	for (i = 0; cur->links[i]; i++) {
		res = dfs_depth(cur->links[i], my_height, sname);
		if (res)
			return 1;
	}
	return 0;
}

void push_stack(nodeptr nptr) {
	top++;
	stack[top] = nptr;
}

void pop_stack() {
	top--;
}

int dfs_ancestors(nodeptr cur, char *sname) {
	int i, res;
	if (strcmp(cur->name, sname) == 0) {
		printf("This person's ancestors: ");
		for (i = 0; i <= top; i++)
			printf("%s   ", stack[i]->name);
		printf("\n");
		return 1;
	}

	push_stack(cur);
	for (i = 0; cur->links[i]; i++) {
		res = dfs_ancestors(cur->links[i], sname);
		if (res)
			return 1;
	}

	pop_stack();
	return 0;
}
void main() {
	char buf[20][50];
	char line[30];
	char command[30];
	char name[30];
	int num_persons, i, k, res;
	nodeptr np;

	FILE *fp;
	fp = fopen("tree_data.txt", "r");
	if (!fp) {
		printf("file open error.\n");
		return;
	}

	do {
		num_persons = read_upper(fp, buf);
		if (num_persons == 0)
			break;
		if (!ROOT) {
			np = (nodeptr)malloc(sizeof(nodetype));
			strcpy(np->name, buf[0]);
			ROOT = np;
			np->links[0] = NULL;
			for (k = 1; k < num_persons; k++)
				make_node_and_attach_to_parent(np, buf[k], k - 1);
			np->links[k - 1] = NULL;
		}
		else {
			np = search_node(ROOT, buf[0]);
			if (!np) {
				printf("Error : 2째 줄 이하의 첫 이름의 노드가 없음.\n");
				getchar();
			}
			for (k = 1; k < num_persons; k++)
				make_node_and_attach_to_parent(np, buf[k], k - 1);
			np->links[k - 1] = NULL;
		}

	} while (1);

	read_store_lower_data(fp);

	print_general_tree(ROOT);

	do {
		printf("Type a command> ");
		gets_s(line);

		i = 0;
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		k = 0;
		while (!(line[i] == ' ' || line[i] == '\t' || line[i] == '\0')) {
			command[k] = line[i];
			i++; k++;
		}
		command[k] = '\0';

		if (strcmp(command, "ex") == 0)
			break;
		else if (strcmp(command, "ht") == 0) {
			int tree_height = 0;
			dfs_height(ROOT, 0, &tree_height);
			printf("Height of the tree : %d \n", tree_height);
			continue;
		}
		else;


		k = 0;
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		while (!(line[i] == ' ' || line[i] == '\t' || line[i] == '\0')) {
			name[k] = line[i];
			i++; k++;
		}
		name[k] = '\0';

		if (strcmp(command, "se") == 0) {
			np = search_node(ROOT, name);
			if (np)
				printf("Search success : %s %5.2f   %s\n", np->name, np->gpa, np->place);
			else
				printf("Such a person does not exist.\n");
		}
		else if (strcmp(command, "dp") == 0) {
			res = dfs_depth(ROOT, 0, name);
			if (!res)
				printf("Such a name does not exist in the tree.\n");
		}
		else if (strcmp(command, "ac") == 0) {
			top = -1;
			res = dfs_ancestors(ROOT, name);
			if (!res)
				printf("Such a name does not exist in the tree.\n");
		}
		else if (strcmp(command, "nm") == 0){
			check = 1;
			res = print_nm(ROOT, name);
			if (!res)
				printf("Such a name does not exist in the tree.\n");
		}
		else if (strcmp(command, "br") == 0) {
			top = -1;
			res = print_bro(ROOT, name);
			if (!res)
				printf("Such a name does not exist in the tree.\n");
		}
		else {
			printf("Wrong command.\n");
		}
	} while (1);
}

int print_nm(nodeptr Root, char *sname) {
	nodeptr curr;
	int i;

	curr = search_node(Root, sname);
	print_nm2(curr);
	printf("%s 및 모든 자손들의 수 : %d명\n", sname, check);

	return 1;
}
void print_nm2(nodeptr curr) {
	int i;
	if (!curr)
		return;
	for (i = 0; curr->links[i]; i++) {
		check++;
		print_nm2(curr->links[i]);
	}
}

void print_tree(nodeptr curr) {
	int i;
	if (!curr)
		return;
	for (i = 0; curr->links[i]; i++) {
		p_bro(curr->links[i]);
	}
}
void p_bro(nodeptr curr) {
	printf("%s    ", curr->name);
}
int print_bro(nodeptr cur, char *sname) {
	int i, res;
	nodeptr cur2;
	if (strcmp(cur->name, sname) == 0) {
		printf("%s의 형제들 : ", sname);
		cur2 = stack[top];
		print_tree(cur2);
		printf("\n");
		return 1;
	}

	push_stack(cur);
	for (i = 0; cur->links[i]; i++) {
		res = print_bro(cur->links[i], sname);
		if (res)
			return 1;
	}

	pop_stack();
	return 0;
}