#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <bitset>

#define cin in

using namespace std;

ifstream in("datedeintrare.txt");
ofstream out("data.txt");
bool* thread_terminat = nullptr;

struct stack {
	int n = -1;
	stack* next = nullptr;
};

struct interval {
	int x, y;
	stack* start;
};

int primver(int n) {
	int i;
	if (n <= 1)
		return 0;

	else if (n < 3)
		return 1;
	else if (n % 2 == 0)
		return 0;
	else {
		for (i = 3; i * i <= n; i = i + 2) {
			if (n % i == 0) {
				return 0;
			}
		}
	}
	return 1;
}

void transfer_date_mem_to_file(interval* trprim,int nrthreaduri) {
	//thread ul asta v-a merge in continu si v-a verifica daca o terminat threadurile
	//dar o sa le ia in ordinea lor
	if (thread_terminat == nullptr)
		return;//fail check
	std::chrono::seconds(4);
	int auterminat = 0;
	while (auterminat < nrthreaduri) {
		if (thread_terminat[auterminat] == 1) {
			stack* mem = trprim[auterminat].start, * aux;
			while (mem->n != -1) {
				out << mem->n << endl;
				aux = mem;
				mem = mem->next;
				delete aux;
			}
			auterminat++;
			continue;
		}
		std::chrono::seconds(4);
	}
	
}

void calc_prim(interval itr,int id) {
	cout << itr.x << " " << itr.y << endl;
	stack* mem = itr.start;

	for (int i = itr.x; i <= itr.y; i++) {
		if (primver(i)) {
			mem->n = i;
			mem->next = new stack;
			mem = mem->next;
		}
	}
	thread_terminat[id] = 1;
}

int main() {
	thread* tr;
	int x, y, r, num, i;
	//preia datele de la tastatura 
	cout << "cate threaduri doresti sa utilizezi ? : ";
	cin >> num;
	cout << "din ce interval doresti sa aflii numerele prime ? " << endl;
	cin >> x >> y;
	num--;//deoarece trebuie un thread sa se ocupe pentru adaugarea 


	//initializarea valorilor si pregatirea variabilelor pentru threaduri
	interval* trprim = new interval[num];
	tr = new thread[num+1];//un thread trebuie sa memoreze datele
	r = (y - x) / num;
	thread_terminat = new bool[num] {0};

	cout << r << endl;


	//se pregateste bucatile din interval
	for (i = 0; i < num - 1; i++) {
		trprim[i].x = (r * i) + x;
		trprim[i].y = x + (r * (i + 1)) - 1;
		trprim[i].start = new stack;
	}
	trprim[i].x = (r * i) + x;
	trprim[i].y = y;
	trprim[i].start = new stack;

	//pentru debugging sau sa se arate cum sunt impartiti
	for (i = 0; i < num; i++) {
		cout << i << '{' << trprim[i].x << "," << trprim[i].y << '}' << endl;
	}


	//seteaza threadurile sa mearga 
	cout << endl << endl;


	//sunt initializate
	for (i = 0; i < num - 1; i++) {
		tr[i] = thread(calc_prim, trprim[i],i);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	tr[i] = thread(calc_prim, trprim[i],i);
	tr[num] = thread(transfer_date_mem_to_file, trprim, num);
	//de aici apare ce s-a intamplat in thread
	for (i = 0; i < num; i++) {
		tr[i].join();
	}
	tr[num].join();
	//aici ar trebuii sa citeasca valorile din stackul din thread
	//dar din pacate nu are cum sa le stearga 
	/*for (i = 0; i < num; i++) {

		stack* mem = trprim[i].start;
		while (mem->n != -1) {
			out << mem->n << endl;
			mem = mem->next;
		}
	}*/

	return 0;
}