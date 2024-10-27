#include <windows.h>
#include <stdio.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

HANDLE mutex;
HANDLE empty;
HANDLE full;

DWORD WINAPI ProducerThread(LPVOID param) {
  int item;
  while (1) {
    // Öğe üret
    item = rand() % 100;

    // Boş semaforu bekle
    WaitForSingleObject(empty, INFINITE);

    // Mutex'i bekle
    WaitForSingleObject(mutex, INFINITE);

    // Arabelleğe ekle
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    count++;

    // Mutex'i serbest bırak
    ReleaseSemaphore(mutex, 1, NULL);

    // Dolu semaforu serbest bırak
    ReleaseSemaphore(full, 1, NULL);

    printf("Uretici: %d ekledi, toplam: %d\n", item, count);
  }
  return 0;
}

DWORD WINAPI ConsumerThread(LPVOID param) {
  int item;
  while (1) {
    // Dolu semaforu bekle
    WaitForSingleObject(full, INFINITE);

    // Mutex'i bekle
    WaitForSingleObject(mutex, INFINITE);

    // Arabellekten al
    item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    count--;

    // Mutex'i serbest bırak
    ReleaseSemaphore(mutex, 1, NULL);

    // Boş semaforu serbest bırak
    ReleaseSemaphore(empty, 1, NULL);

    printf("Tuketici: %d aldi, toplam: %d\n", item, count);
  }
  return 0;
}

int main() {
  // Semaforları oluştur
  mutex = CreateSemaphore(NULL, 1, 1, NULL);
  empty = CreateSemaphore(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
  full = CreateSemaphore(NULL, 0, BUFFER_SIZE, NULL);

  // Threadleri oluştur
  HANDLE producerThread = CreateThread(NULL, 0, ProducerThread, NULL, 0, NULL);
  HANDLE consumerThread = CreateThread(NULL, 0, ConsumerThread, NULL, 0, NULL);

  // Threadlerin bitmesini bekle
  WaitForSingleObject(producerThread, INFINITE);
  WaitForSingleObject(consumerThread, INFINITE);

  // Semaforları kapat
  CloseHandle(mutex);
  CloseHandle(empty);
  CloseHandle(full);

  return 0;
}
