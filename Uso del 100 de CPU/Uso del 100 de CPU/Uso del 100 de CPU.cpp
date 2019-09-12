// Uso del 100 de CPU.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <thread>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <mutex>
#include <future>

using namespace std;

int prime(int a, int b)
{
	int primes = 0;
	for (a; a <= b; a++) {
		int i = 2;
		while (i <= a) {
			if (a % i == 0)
				break;
			i++;
		}
		if (i == a) {
			primes++;
		}
	}
	return primes;
}


int workConsumingPrime(vector<pair<int, int>>& workQueue, mutex& workMutex)
{
	int primes = 0;
	unique_lock<mutex> workLock(workMutex);
	while (!workQueue.empty()) {
		pair<int, int> work = workQueue.back();
		workQueue.pop_back();

		workLock.unlock(); //< Don't hold the mutex while we do our work.
		primes += prime(work.first, work.second);
		workLock.lock();
	}
	return primes;
}


int main()
{
	int nthreads = thread::hardware_concurrency();
	int limit = 1000000;

	// A place to put work to be consumed, and a synchronisation object to protect it.
	vector<pair<int, int>> workQueue;
	mutex workMutex;

	// Put all of the ranges into a queue for the threads to consume.
	int chunkSize = max(limit / (nthreads * 16), 10); //< Handwaving came picking 16 and a good factor.
	for (int i = 0; i < limit; i += chunkSize) {
		workQueue.push_back(make_pair(i, min(limit, i + chunkSize)));
	}

	// Start the threads.
	vector<future<int>> futures;
	for (int i = 0; i < nthreads; ++i) {
		packaged_task<int()> task(bind(workConsumingPrime, ref(workQueue), ref(workMutex)));
		futures.push_back(task.get_future());
		thread(move(task)).detach();
	}

	cout << "Number of logical cores: " << nthreads << "\n";
	cout << "Calculating number of primes less than " << limit << "... \n";

	// Sum up all the results.
	int primes = 0;
	for (future<int>& f : futures) {
		primes += f.get();
	}

	cout << "There are " << primes << " prime numbers less than " << limit << ".\n";
}
// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
