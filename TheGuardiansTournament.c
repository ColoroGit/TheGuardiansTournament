#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct guardian
{
	char name[50]; //Nombre del guardian
	char type[20]; //tipo del guardian
	int id; //identificador único del guardian, sirve para que algunas funciones sean más simples de realizar
	int hp; //puntos de vida del guardian
	int dmg; //puntos de daño del guardian
	int defense; //puntos de defensa del guardian
	struct guardian *next; //la direccion de memoria del guardian que le sigue en la cola
} Guardian;

//estructura que registra los combates realizados
typedef struct combat
{
	char tournament[20]; //torneo en el que peleo este Battle
	int numBattle; //que combate del torneo era
	int turns; //la cantidad de rondas que le tomó esta pelea
	Guardian *opponent; //el oponente de esa pelea
	Guardian *player; //cómo terminó el player esa pelea (dependiendo de si murió o no dirá que se perdió o se mostraran las estadisticas cuando venció al oponente)
	struct combat *next; //la direccion de memoria del combate que le sigue
} Combat;

Guardian* ChargeGuardian(char name[], char type[], int id, int hp, int dmg, int defense) //función para cargar a los Guardianes desde un archivo
{
	Guardian *newGuardian = (Guardian*)malloc(sizeof(Guardian));
	strcpy(newGuardian->name, name);
	strcpy(newGuardian->type, type);
	newGuardian->id = id;
	newGuardian->hp = hp;
	newGuardian->dmg = dmg;
	newGuardian->defense = defense;
	newGuardian->next = NULL;
	
	return newGuardian;
}

Combat* ChargeCombat(char tournament[], int numBattles, int turns, Guardian *player, Guardian *opponent)
{
	Combat *newCombat = (Combat*)malloc(sizeof(Combat));
	
	strcpy(newCombat->tournament, tournament);
	newCombat->numBattle = numBattles;
	newCombat->turns = turns;
	newCombat->player = player;
	newCombat->opponent = opponent;
	newCombat->next = NULL;
	
	return newCombat;
}

void Enqueue(Guardian **head, Guardian *newGuardian) //Clásica función para agregar a una estructura al final de una cola
{
	if (*head == NULL)
	{
		*head = newGuardian;
	}
	else
	{
		Guardian *current = *head;
		
		while (current->next != NULL)
		{
			current = current->next;
		}
		
		current->next = newGuardian;
	}
}

Guardian* Dequeue(Guardian **head) //Conocida función para sacar y retornar al primer objeto de la cola
{
	Guardian *aux = *head;
	*head = aux->next;
	aux->next = NULL;
	return aux;
}

Guardian* ReturnElement(Guardian **head, Guardian *g) //Retorna y saca de la lista al guardian que coincida con el guardian entregado
{
	if (g == *head)
	{
		return Dequeue(head);
	}
	else
	{
		Guardian *current = *head;
		
		while (current->next != g)
		{
			current = current->next;
		}	
		
		Guardian *aux = current->next;
		current->next = aux->next;
		aux->next = NULL;
		return aux;
	}
}

Guardian* GetElement(Guardian **head, Guardian *g) //A diferencia de la funcion de arriba, esta no saca al Guardian de la fila o cola a la que pertenece
{
	if (g == *head)
	{
		return *head;
	}
	else
	{
		Guardian *current = *head;
		
		while (current != g)
		{
			current = current->next;
		}	
		
		return current;
	}
}

void EnqueueCombat (Combat **head, Combat *newCombat) //Clásica función para agregar un elemento a una pila
{
	if (*head == NULL)
	{
		*head = newCombat;
	}
	else
	{
		Combat *current = *head;
		
		while (current->next != NULL)
		{
			current = current->next;
		}
		
		current->next = newCombat;
	}
}

void PrintGuardians(Guardian *head, int i) //Función para imprimir a toda o parte de la cola de guardianes
{
	Guardian *current = head;
	
	while (i > 0 && current != NULL)
	{
		printf("\n   Guardi\xA0n %d:\n Nombre: %s\n Tipo: %s\n Vida: %d\n Da\xA4o: %d\n Defensa: %d\n", current->id, current->name, current->type, current->hp, current->dmg, current->defense);
		current = current->next;
		i--;
	}
}

void PrintGuardian(Guardian *g) //Función para imprimir a un solo guardian
{
	printf("\n\t***Estadisticas del Guardi\xA0n***\n");
	printf("\n Nombre: %s\n Tipo: %s\n Vida: %d\n Da\xA4o: %d\n Defensa: %d\n", g->name, g->type, g->hp, g->dmg, g->defense);
}

void Randomize(Guardian **head, int i) //Función para desordenar a los guardianes ubicados en una cola
{
	srand(time(NULL)); //Lo que hace esta funcion es tomar a un guardian cualquiera dentro de la cola y mandarlo al final de esta, luego de nuevo, pero sin contar a los que ya han sido movidos.
	
	while (i > 0)
	{
		Guardian *current = *head;
		int j = rand() % i;
		
		while (j > 0)
		{
			current = current->next;
			j--;
		}
		
		Guardian *aux = current->next;
		current->next = aux->next;
		aux->next = NULL;
		Enqueue(head, aux);
		i--;
	}
}

Guardian* CreateGuardian(char name[], int op) //Funcion que crea a un guardián en base a datos entregados por el usuario, y otros que se generan aleatoriamente
{
	srand(time(NULL));
	Guardian *newGuardian = (Guardian*)malloc(sizeof(Guardian));
	
	newGuardian->id = 0;
	strcpy(newGuardian->name, name);
	
	switch (op)
	{
		case 1:
		strcpy(newGuardian->type, "mage");
		break;
		
		case 2:
		strcpy(newGuardian->type, "nigromante");
		break;
		
		case 3:
		strcpy(newGuardian->type, "beast");
		break;
		
		case 4:
		strcpy(newGuardian->type, "viking");
		break;
	}
	
	newGuardian->hp = (((rand() % 21) + 40) * 10);
	newGuardian->dmg = (((rand() % 21) + 20) * 5);
	newGuardian->defense = (((rand() % 15) + 6) * 5);
	newGuardian->next = NULL;
	
	return newGuardian;
}

Guardian* SelectGuardian(Guardian **head, int i) //Función para que el player seleccione a un guardian 
{
	Guardian *current = *head;
	int op;
	int aux = i;
	
	while (op != -1)
	{
		printf("\nElija a su Guardi\xA0n ingresando su n\xA3mero correspondiente\no <-1> para volver al men\xA3: ");
		scanf("%d", &op);
		
		i = aux;
			
		while (i > 0)
		{
			if (current->id == op)
			{
				return ReturnElement(head, current);
			}
			current = current->next;
			i--;
		}
		
		printf("Ese Guardi\xA0n no existe, por favor ingrese otro n\xA3mero");
	}
	
	return NULL;
}

Guardian* CopyGuardian(Guardian *g) //Función que crea una copia del guardian entregado
{
	if (g != NULL)
	{
		Guardian *copy = (Guardian*)malloc(sizeof(Guardian));
	
		strcpy(copy->name, g->name);
		strcpy(copy->type, g->type);
		copy->id = g->id;
		copy->hp = g->hp;
		copy->dmg = g->dmg;
		copy->defense = g->defense;
		copy->next = g->next;
		
		return copy;
	}
	
	return NULL;
}

bool Battle(Guardian *player, Guardian *opponent, int numBattle, Combat *newCombat) //Función que inicia y simula el combate entre el jugador y su oponente
{
	srand(time(NULL));
	Guardian *current = (Guardian*)malloc(sizeof(Guardian)); //variable que contiene al guardian que está actualmente jugando
	int turn = 1;
	int valor;
	int op;
	int dado;
	bool extra = false; //variable que representa si se está en el turno extra o no
	
	while (player->hp > 0 && opponent->hp > 0)
	{
		if (turn == 1) //Asignación de quién parte
		{
			if (rand() % 2 == 0)
			{
				current = player;
			}
			else
			{
				current = opponent;
			}	
		}
		
		printf("\n\t\t*****Turno %d*****\n", turn);
		printf("\nT\xA3: ");
		PrintGuardian(player);
		printf("\nTu oponente: ");
		PrintGuardian(opponent);
		system("pause");
		
		if (current == player) //Turno del player
		{
			printf("\n\t***Te toca %s***\n", player->name);
			printf("Qu\x82 deseas hacer?\n   (1) Atacar\n   (2) Sanarte\nIngresa el n\xA3mero entre per\x82ntesis de tu selecci\xA2n: ");
			scanf("%d", &op);
			
			while (op != 1 && op != 2)
			{
				printf("Esa opci\xA2n no existe, por favor selecciona otra: ");
				scanf("%d", &op);
			}
			
			if (op == 1) //ATACAR
			{
				printf("\nPresiona enter para lanzar el dado");
				fflush(stdin);
				getchar(); //esto está pensado únicamente para hacer una pausa y que haya interacción por parte del usuario
				dado = (rand() % 6) + 1;
				printf("Has obtenido un %d!", dado);
				
				switch (dado)
				{
					case 1:
					valor = (int)(player->dmg * 0.8);
					opponent->hp -= valor;
					printf("\n\tAtaque exitoso!!!\nLe has quitado %d puntos de vida a tu oponente\n", valor);
					
					if (opponent->hp < 0)
					{
						opponent->hp = 0;
						printf("\n\tOponente Abatido!!!\n");
					}
					break;
					
					case 3:
					valor = player->dmg;
					opponent->hp -= valor;
					printf("\n\tAtaque exitoso!!!\nLe has quitado %d puntos de vida a tu oponente\n", valor);
					
					if (opponent->hp < 0)
					{
						opponent->hp = 0;
						printf("\n\tOponente Abatido!!!\n");
					}
					break;
					
					case 5:
					valor = (int)(player->dmg * 1.3);
					opponent->hp -= valor;
					printf("\n\tAtaque exitoso!!!\nLe has quitado %d puntos de vida a tu oponente\n", valor);
					
					if (opponent->hp < 0)
					{
						opponent->hp = 0;
						printf("\n\tOponente Abatido!!!\n");
					}
					break;
					
					default:
					printf("\nTu ataque ha fallado, pero no te rindas todavia!!!\n");
					break;
				}
			}
			else //SANARSE
			{
				printf("\nPresiona enter para lanzar el dado");
				fflush(stdin);
				getchar(); //esto está pensado únicamente para hacer una pausa y que haya interacción por parte del usuario
				dado = (rand() % 6) + 1;
				printf("Has obtenido un %d", dado);
				
				switch (dado)
				{
					case 2:
					valor = (int)(player->defense * 0.5);
					player->hp += valor; 
					printf("\n\tSanaci\xA2 exitosa!!!\nHas recuperado %d puntos de vida\n", valor);
					break;
					
					case 4:
					valor = player->defense;
					player->hp += valor; 
					printf("\n\tSanaci\xA2 exitosa!!!\nHas recuperado %d puntos de vida\n", valor);
					break;
					
					case 6:
					valor = (int)(player->defense * 1.2);
					player->hp += valor; 
					printf("\n\tSanaci\xA2 exitosa!!!\nHas recuperado %d puntos de vida\n", valor);
					break;
					
					default:
					printf("\nTu sanaci\xA2n ha fallado\n");
					if (player->defense > 30)
					{
						valor = (int)(player->defense * 0.05);
						player->defense -= valor;
						printf("has recibido un desgaste en tu defensa de %d puntos!\n", valor);
						
						if (player->defense < 30)
						{
							player->defense = 30;
							printf("\nTu defensa ha llegado al minimo (no recibir\xA0s m\xA0s desgaste)\n");
						}
					}
					break;
				}
			}
			
			if (dado == 6 && !extra)
			{
				extra = true;
				printf("\nComo obtuviste un 6 en el dado, tienes un turno extra!!!\n");
			}
			else
			{
				extra = false;
				current = opponent;
			}
			
			system("pause");
		}
		else //Juega la máquina
		{
			printf("\n\t***Le toca a tu oponente %s***\n", opponent->name);
			
			op = (rand() % 2) + 1;
			
			if (op == 1) //ATACAR
			{
				dado = (rand() % 6) + 1;
				printf("El enemigo a decidido Atacar y obtuvo un %d en el dado\n", dado);
				
				switch (dado)
				{
					case 1:
					valor = (int)(opponent->dmg * 0.8);
					player->hp -= valor;
					printf("\n\tAtaque exitoso!!!\nTu oponente te ha quitado %d puntos de vida\n", valor);
					
					if (player->hp < 0)
					{
						player->hp = 0;
						printf("\n\tHas sido derrotado\n");
					}
					break;
					
					case 3:
					valor = opponent->dmg;
					player->hp -= valor;
					printf("\n\tAtaque exitoso!!!\nTu oponente te ha quitado %d puntos de vida\n", valor);
					
					if (player->hp < 0)
					{
						player->hp = 0;
						printf("\n\tHas sido derrotado\n");
					}
					break;
					
					case 5:
					valor = (int)(opponent->dmg * 1.3);
					player->hp -= valor;
					printf("\n\tAtaque exitoso!!!\nTu oponente te ha quitado %d puntos de vida\n", valor);
					
					if (player->hp < 0)
					{
						player->hp = 0;
						printf("\n\tHas sido derrotado\n");
					}
					break;
					
					default:
					printf("\nEl ataque de %s ha fallado!!!\n", opponent->name);
					break;
				}
			}
			else //SANARSE
			{
				dado = (rand() % 6) + 1;
				printf("El enemigo a decidido Sanarse y obtuvo un %d en el dado\n", dado);
				
				switch (dado)
				{
					case 2:
					valor = (int)(opponent->defense * 0.5);
					opponent->hp += valor; 
					printf("\n\tSanaci\xA2 exitosa!\nTu oponente ha recuperado %d puntos de vida\n", valor);
					break;
					
					case 4:
					valor = opponent->defense;
					opponent->hp += valor; 
					printf("\n\tSanaci\xA2 exitosa!\nTu oponente ha recuperado %d puntos de vida\n", valor);
					break;
					
					case 6:
					valor = (int)(opponent->defense * 1.2);
					opponent->hp += valor; 
					printf("\n\tSanaci\xA2 exitosa!\nTu oponente ha recuperado %d puntos de vida\n", valor);
					break;
					
					default:
					printf("\nLa sanaci\xA2n de %s ha fallado!!!", opponent->name);
					if (opponent->defense > 30)
					{
						valor = (int)(opponent->defense * 0.05);
						opponent->defense -= valor;
						printf("\nY ha recibido un desgaste de %d puntos en su defensa!!!\n", valor);
						
						if (opponent->defense < 30)
						{
							opponent->defense = 30;
							printf("\nLa defensa de %s ha llegado al minimo (no recibir\xA0 m\xA0s desgaste)\n", opponent->name);
						}
					}
					break;
				}
			}
			
			if (dado == 6 && !extra) 
			{
				extra = true;
				printf("\nComo %s obtuvo un 6 en el dado, tiene un turno extra!\n", opponent->name);
			}
			else
			{
				extra = false;
				current = player; 
			}
			
			system("pause");
		}
		
		turn++;
	}
		
	//Registro final del combate
	turn--;
	newCombat->turns = turn;
	newCombat->player = player;
	newCombat->next = NULL;
	
	if (player->hp <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool IniciarTorneo(Guardian **head, Guardian *player, char tournament[], Combat **top) //Funcion que inicia el torneo que se haya elegido, llevando a los combates correspondientes
{
	//Crear copias de los peleadores, para poder conservar a los originales y se pueda simular la batalla
	Guardian *opponent = CopyGuardian(*head);
	Guardian *copyPlayer = CopyGuardian(player);
	int i = 1;
	bool victory = true;
	
	while (opponent != NULL && victory) //inician las batallas del torneo
	{
		Combat *newCombat = (Combat*)malloc(sizeof(Combat));
		
		//Registro inicial del combate
		newCombat->opponent = CopyGuardian(opponent);
		newCombat->numBattle = i;
		strcpy(newCombat->tournament, tournament);
		
		//Inicio del combate
		printf("\n\t***Que comience el combate n\xA3mero %d***\n", i);
		system("pause");
		
		victory = Battle(copyPlayer, opponent, i, newCombat);
		EnqueueCombat(top, newCombat); //Guardado de la batalla en el historial
		
		if (victory)
		{
			opponent = CopyGuardian(GetElement(head, opponent->next)); //Obtener una copia del siguiente guardian con quien pelear
			copyPlayer = CopyGuardian(player); //Y restaurar al jugador
			i++;
		}
	}
	
	return victory;
}

void ViewHistory(Combat *head) //Función que muestra todo el registro de combate del jugador
{
	if (head != NULL)
	{
		Combat *current = head;
		
		printf("\n\t\t\t*****La Historia de %s*****\n", current->player->name);
	
		while (current != NULL)
		{
			printf("\n\t\t***Torneo %s, Combate N\xA3mero %d:***\n\nTu oponente era %s: \n", current->tournament, current->numBattle, current->opponent->name);
			PrintGuardian(current->opponent);
			
			if (current->player->hp > 0)
			{
				printf("\nTe tom\xA2 %d turnos vencer a %s, y estas fueron tus estadisticas finales: \n", current->turns, current->opponent->name);
				PrintGuardian(current->player);
			}
			else
			{
				printf("\nY perdiste esta batalla en el turno %d\n", current->turns);
			}
			
			current = current->next;
		}
	}
	else
	{
		printf("\n\t\tTu historial est\xA0 vacio...por ahora...\n\tVe a competir en los torneos para hacer de tu Guardi\xA0n una leyenda!!!\n");
	}
}

void FreeGuardians(Guardian *head) //Función para liberar la memoria de los guardianes
{
	Guardian *current = head;
	
	while (current != NULL) 
	{
		Guardian *next = current->next;
		free(current);
		current = next;
	}
}

void FreeCombats(Combat *head) //Función para liberar la memoria de los combates, del historial
{
	Combat *current = head;
	
	while (current != NULL) 
	{
		Combat *next = current->next;
		free(current);
		current = next;
	}
}

void SaveGame(Combat *head, Guardian *player, Guardian *torneoPrincipiante, Guardian *torneoIntermedio, Guardian *torneoExperto)
{
	FILE *file = fopen("PartidaGuardada.txt", "w");
	
	//Guardado de los Guardianes que ya están en cada torneo
	Guardian *currentGuardian = torneoPrincipiante;
	
	while (currentGuardian != NULL)
	{
		fprintf(file, "%s,%s,%d,%d,%d,%d\n", currentGuardian->name, currentGuardian->type, currentGuardian->id, currentGuardian->hp, currentGuardian->dmg, currentGuardian->defense);
		currentGuardian = currentGuardian->next;
	}
	
	currentGuardian = torneoIntermedio;
	
	while (currentGuardian != NULL)
	{
		fprintf(file, "%s,%s,%d,%d,%d,%d\n", currentGuardian->name, currentGuardian->type, currentGuardian->id, currentGuardian->hp, currentGuardian->dmg, currentGuardian->defense);
		currentGuardian = currentGuardian->next;
	}
	
	currentGuardian = torneoExperto;
	
	while (currentGuardian != NULL)
	{
		fprintf(file, "%s,%s,%d,%d,%d,%d\n", currentGuardian->name, currentGuardian->type, currentGuardian->id, currentGuardian->hp, currentGuardian->dmg, currentGuardian->defense);
		currentGuardian = currentGuardian->next;
	}
	
	//Guardado del Guardián que eligió o creó el jugador
	currentGuardian = player;
	fprintf(file, "%s,%s,%d,%d,%d,%d\n", currentGuardian->name, currentGuardian->type, currentGuardian->id, currentGuardian->hp, currentGuardian->dmg, currentGuardian->defense);
	
	Combat *current = head;
	
	while (current != NULL) //Guardado del historial
	{
		fprintf(file, "%s,%d,%d\n", current->tournament, current->numBattle, current->turns);
		fprintf(file, "%s,%s,%d,%d,%d,%d\n", current->player->name, current->player->type, current->player->id, current->player->hp, current->player->dmg, current->player->defense);
		fprintf(file, "%s,%s,%d,%d,%d,%d\n", current->opponent->name, current->opponent->type, current->opponent->id, current->opponent->hp, current->opponent->dmg, current->opponent->defense);
		current = current->next;
	}
	
	printf("\n\t***Partida Guardada***\n");
	printf("\nIMPORTANTE: Cuando quieras cargar la partida al abrir el juego, deber\xA0s escribir lo siguiente: \n.%cTheGuardiansTournament Guardians.txt PartidaGuardada.txt\n(Se recomienda guardar una foto o copiar el texto)\n", 92);
	system("pause");
}

int main(int argc, char *argv[]) 
{
	//Inicialización de variables 
	Guardian *guardians = NULL; //cola de guardianes
	Combat *history = NULL; //pila de registro de combates
	
	Guardian *torneoPrincipiante = NULL;
	Guardian *torneoIntermedio = NULL; //Creación de los torneos, tienen estos nombres por convención respecto a lo solicitado en el proyecto, y para facilitar la comprensión, que consideré que tenía mayor prioridad e importancia que mantener el idioma en inglés
	Guardian *torneoExperto = NULL;
	
	Guardian *player = (Guardian*)malloc(sizeof(Guardian));
	player = NULL;
	
	char name[50];
	char type[20];
	bool torneosSuperados[3] = {false, false, false};
	int i = 1;
	int op;
	bool exit = false;
	
	if (argc == 3) //Cargado de partida si es que el usuario ingresa un tercer argumento en consola
	{
		char *fileName = argv[2];
		FILE *file = fopen(fileName, "r");
		
		if (file == NULL) 
		{
			printf("Error, No se pudo cargar la partida, el nombre del archivo ingresado no existe");
			return 1;
		}
		
		char str[100];
		char *token;
		
		//Variables para la creación de las estructuras
		char tournament[20];
		char nameO[50];
		char typeO[20];
		
		printf("\nCargando Partida...\n");
		int j = 0;
		
		while (j < 15) //Carga de los Guardianes en el orden que se guardaron, para poder mantener los torneos con los mismos guardianes
		{
			fgets(str, 100, file);
			
			token = strtok(str, ",");
			strcpy(name, token);
			
			token = strtok(NULL, ",");
			strcpy(type, token);
			
			i = atoi(strtok(NULL, ","));
			int hp = atoi(strtok(NULL, ","));
			int dmg = atoi(strtok(NULL, ","));
			int defense = atoi(strtok(NULL, ","));
			
			Enqueue(&guardians, ChargeGuardian(name, type, i, hp, dmg, defense));
			j++;
		}
		
		//Carga del Guardián seleccionado o creado por el usuario
		fgets(str, 100, file);
		
		token = strtok(str, ",");
		strcpy(name, token);
		
		token = strtok(NULL, ",");
		strcpy(type, token);
		
		i = atoi(strtok(NULL, ","));
		int hp = atoi(strtok(NULL, ","));
		int dmg = atoi(strtok(NULL, ","));
		int defense = atoi(strtok(NULL, ","));
		
		player = ChargeGuardian(name, type, i, hp, dmg, defense);
		
		while (fgets(str, 100, file) != NULL) //Carga del historial
		{
			//Cargando combate
			token = strtok(str, ",");
			strcpy(tournament, token);
			
			int numBattle = atoi(strtok(NULL, ","));
			int turns = atoi(strtok(NULL, ","));
			
			fgets(str, 100, file); //Siguiente Linea
			
			//Cargando Jugador
			token = strtok(str, ",");
			strcpy(name, token);
			
			token = strtok(NULL, ",");
			strcpy(type, token);
			
			int iP = atoi(strtok(NULL, ",")); //P de PLayer
			int hpP = atoi(strtok(NULL, ","));
			int dmgP = atoi(strtok(NULL, ","));
			int defenseP = atoi(strtok(NULL, ","));
			
			//Los siguientes 3 ifs son para comprobar si los torneos ya fueron superados por el jugador
			if (strcmp(tournament, "Principiante") == 0 && numBattle == 3 && hpP != 0)
			{
				torneosSuperados[0] = true;
			}
			
			if (strcmp(tournament, "Intermedio") == 0 && numBattle == 5 && hpP > 0)
			{
				torneosSuperados[1] = true;
			}
			
			if (strcmp(tournament, "Experto") == 0 && numBattle == 7 && hpP > 0)
			{
				torneosSuperados[2] = true;
			}
			
			fgets(str, 100, file); //Siguiente Linea
			
			//Cargando Oponente
			token = strtok(str, ",");
			strcpy(nameO, token);
			
			token = strtok(NULL, ",");
			strcpy(typeO, token);
			
			int iO = atoi(strtok(NULL, ","));; //O de Opponent
			int hpO = atoi(strtok(NULL, ","));
			int dmgO = atoi(strtok(NULL, ","));
			int defenseO = atoi(strtok(NULL, ","));
			
			EnqueueCombat(&history, ChargeCombat(tournament, numBattle, turns, ChargeGuardian(name, type, iP, hpP, dmgP, defenseP), ChargeGuardian(nameO, typeO, iO, hpO, dmgO, defenseO)));
		}
		
		printf("\n\t*****Partida Cargada*****\n");
		system("pause");
		fclose(file);
	}
	else //El código que viene a continuación es para cargar en el programa el archivo Guardians.txt, donde viene la información de los guardianes
	{
		char *file_name = argv[1];
		FILE *file = fopen(file_name, "r");
		
		if (file == NULL) 
		{
			fprintf(stderr, "Error: Could not open file \n");
			return 1;
		}
		
		char line[100];
		
		while (fgets(line, 100, file) != NULL) 
		{
			char *token;
			
			token = strtok(line, ",");
			strcpy(name, token);
			
			token = strtok(NULL, ",");
			strcpy(type, token);
			
			int hp = atoi(strtok(NULL, ","));
			int dmg = atoi(strtok(NULL, ","));
			int defense = atoi(strtok(NULL, ","));
			
			Enqueue(&guardians, ChargeGuardian(name, type, i, hp, dmg, defense));
			i++;
		}
		
		fclose(file);
		i--;
		
		Randomize(&guardians, i);
		Enqueue(&guardians, Dequeue(&guardians)); //esto es para mover al primero de la cola al final, ya que es el único que no se mueve durante la randomización
		Randomize(&guardians, i);
	}
	
	for (i = 0; i < 3; i++) //Carga de los guardianes en los torneos
	{
		Enqueue(&torneoPrincipiante, Dequeue(&guardians));
	}
	
	for (i = 0; i < 5; i++)
	{
		Enqueue(&torneoIntermedio, Dequeue(&guardians));
	}
	
	for (i = 0; i < 7; i++)
	{
		Enqueue(&torneoExperto, Dequeue(&guardians));
	}
	
	//Bienvenida del programa (sacada del documento del Proyecto de Unidad, elaborado por Manuel Moscoso)
	printf("\n\t*****Bienvenido a The Guardians Tournament*****\n");
	printf("\nEn un mundo devastado por la guerra y la lucha constante, donde la magia es real, existen los Guardianes.\nLos Guardianes son h\x82roes con habilidades y destrezas \xA3nicas que protegen a las aldeas a lo largo de toda la tierra,\nsiendo estas los \xA3ltimos asentamientos de la humanidad. \nTodos los a\xA4os se realiza el torneo de los guardianes, donde se busca encontrar al campe\xA2n,\nquien gozar\xA0 del reconocimiento y la gloria eterna.\n");
	system("pause");
	
	//Menú de selección
	while (!exit)
	{
		printf("\n\t***Men\xA3 Principal***\n");
		
		printf("\nQu\x82 desea realizar?\n\n   (1) Crear un Guardi\xA0n\n   (2) Seleccionar un Guardi\xA0n preexistente\n   (3) Seleccionar Torneo\n   (4) Ver Resultados Previos\n   (5) Tutorial\n   (6) Guardar Partida\n   (7) Salir del Juego\n");
		printf("\nIngrese el n\xA3mero entre parentesis de la acci\xA2n: ");
		scanf("%d", &op);
		
		if (op > 0 && op < 8)
		{
			switch (op)
			{
				case 1: //Crear Guardian
				if (player == NULL)
				{
					printf("\n\t***Creando a un Guardi\xA0n***\n\nA continuaci\xA2n deber\xA0s ingresar el nombre y el tipo de tu Guardi\xa0n.\nEl resto de estadisticas ser\xA0n asignadas aleatoriamente.");
					printf("\nIngrese el nombre de su Guardi\xA0n: ");
					scanf("%s", name);
					printf("Seleccione el tipo de su Guardi\xA0n:   (1) Mago   (2) Nigromante   (3) Bestia   (4) Vikingo");
					printf("\nIngrese el n\xA3mero entre parentesis del que eligi\xA2: ");
					scanf("%d", &op);
					
					while (op < 1 || op > 4)
					{
						printf("Ese tipo no existe, por favor ingrese otro: ");
						scanf("%d", &op);
					}
					
					player = CreateGuardian(name, op);
					
					printf("\n\t***Guardi\xA0n Creado***\n");
					PrintGuardian(player);
				}
				else
				{
					printf("\n\t***Ya has seleccionado a tu Guardi\xA0n, no puedes elegir otro***\n");
				}
				break;
				
				case 2: //Seleccionar Guardian
				if (player == NULL)
				{
					printf("\n\t***Seleccione a su Guardi\xA0n***\n");
					PrintGuardians(guardians, 5);
					player = SelectGuardian(&guardians, 5);
					
					printf("\n\t***Guardi\xA0n Seleccionado***\n");
					PrintGuardian(player);
				}
				else
				{
					printf("\n\t***Ya has seleccionado a tu Guardi\xA0n, no puedes elegir otro***\n");
				}
				break;
				
				case 3: //Seleccionar Torneo
				if (player != NULL)
				{
					printf("\nSeleccione la dificultad del torneo:\n\n   (1)   Principiante\nObjetivo: derrotar a 3 Guardianes\n   (2)   Intermedio\nObjetivo: derrotar a 5 Guardianes\n   (3)   Experto\nObjetivo: derrotar a 7 Guardianes\n");
					printf("\nPara elegir la dificultad, ingrese el n\xA3mero entre parentesis de su selecci\xA2n\no <-1> para volver al men\xA3: ");
					scanf("%d", &op);
					
					if (op != -1)
					{
						while (op < 1 || op > 3)
						{
							printf("Esa opci\xA2n es inv\xA0lida, por favor ingrese otra: ");
							scanf("%d", &op);
							
							if (op == -1)
							{
								break;
							}
						}
					}
					
					if (op != -1)
					{
						switch (op)
						{
							case 1:
							if (!torneosSuperados[op - 1])
							{
								torneosSuperados[op - 1] = IniciarTorneo(&torneoPrincipiante, player, "Principiante", &history); 
							}
							else
							{
								printf("\nYa has completado y ganado este torneo, prueba las dem\xA0s dificultades, \no reinicia el juego (sin cargar partida) para empezar desde 0.\n");
							}
							break;
							
							case 2:
							if (!torneosSuperados[op - 1])
							{
								torneosSuperados[op - 1] = IniciarTorneo(&torneoIntermedio, player, "Intermedio", &history);
							}
							else
							{
								printf("\nYa has completado y ganado este torneo, prueba las dem\xA0s dificultades, \no reinicia el juego (sin cargar partida) para empezar desde 0.\n");
							}
							break;
							
							case 3:
							if (!torneosSuperados[op - 1])
							{
								torneosSuperados[op - 1] = IniciarTorneo(&torneoExperto, player, "Experto", &history);
							}
							else
							{
								printf("\nYa has completado y ganado este torneo, prueba las dem\xA0s dificultades, \no reinicia el juego (sin cargar partida) para empezar desde 0.\n");
							}
							break;
						}
					}
				}
				else
				{
					printf("\n\t***No puedes iniciar un torneo si no has seleccionado o creado a un Guardi\xA0n***\n");
				}
				break;
				
				case 4: //Ver resultados
				ViewHistory(history);
				system("pause");
				break;
				
				case 5: //Tutorial
				printf("\nThe Guardians Tournament es un juego de peleas por turnos, donde usando a tu guardi\xA0n, \nte enfrentar\xA0s al resto de guardianes en distintos torneos, clasificados seg\xA3n su dificultad.\n\nDurante el combate, tendr\xA0s la opci\xA2n de Atacar o Sanarte.\n\nSi decides atacar, se lanzar\xA0 un dado de 6 caras, y si obtienes un n\xA3mero impar,\ntu ataque ser\xA0 exitoso, de lo contrario fallar\xA0s.\nSi sacaste un 1 en el dado, tu estadistica de Da\xA4o se multiplicar\xA0 por un factor de 0.8, \ny ese ser\xa0 el da\xA4o que ejerzas a tu enemigo.\nSi es un 3, el factor ser\xA0 un 1, y si es 5, el factor ser\xA0 un 1.3.\n\nSi decides sanarte, se lanzar\xA0 un dado de 6 caras, y si es par, tu sanaci\xA2n ser\xA0 exitosa, \nde lo contrario recibir\xA0s un desgaste en tu capacidad de sanaci\xA2n, que se ver\xA0 reflejado en tus puntos de defensa.\nSi obtuviste un 2, tu estadistica de Defensa se multiplicar\xA0 por un 0.5,\ny esa ser\xA0 la sanaci\xA2n que recibas\nSi es un 4, el factor ser\xA0 un 1, y si es un 6, el factor ser\xA0 un 1.2.\n");
				printf("\nSi sacas un 6, independiente de tu acci\xA2n, tendr\xA0s un turno extra, para atacar o sanarte nuevamente, \npero s\xA2lo obtienes un turno extra por jugada, si vuelves a sacar un segundo 6 consecutivo, no tendr\xA0s jugada extra.\n");
				printf("\nUna vez terminado el torneo, podr\xA0s revisar tus resultados en el men\xA3 principal, y, si lo deseas,\ncompetir en otro torneo de mayor o menor dificultad.\n");
				system("pause");
				break;
				
				case 6:
				SaveGame(history, player, torneoPrincipiante, torneoIntermedio, torneoExperto);
				break;
				
				case 7: //Salir del programa
				exit = true;
				break;
			}
		}
		else
		{
			printf("\n\t***Esa opci\xA2n no existe, ingrese otra por favor***\n");
		}
	}
	
	//Liberación de memoria
	FreeGuardians(guardians);
	FreeGuardians(torneoPrincipiante);
	FreeGuardians(torneoIntermedio);
	FreeGuardians(torneoExperto);
	FreeCombats(history);
		
	printf("\n\t*****Gracias por Jugar*****\n\n");
	
	return 0;
}

