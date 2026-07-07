#include <string.h>
#include <stdlib.h>
#include <stdio.h>


struct listing { 
  int id, host_id, minimum_nights, number_of_reviews, calculated_host_listings_count, availability_365;
  char *host_name, *neighbourhood_group, *neighbourhood, *room_type; 
  float latitude, longitude, price; 
};

struct listing getfields(char* line){ 
   struct listing item; 

   item.id = atoi(strtok(line, ",")); 
   item.host_id = atoi(strtok(NULL, ",")); 
   item.host_name = strdup(strtok(NULL, ",")); 
   item.neighbourhood_group = strdup(strtok(NULL, ",")); 
   item.neighbourhood = strdup(strtok(NULL, ",")); 
   item.latitude = atof(strtok(NULL, ",")); 
   item.longitude = atof(strtok(NULL, ",")); 
   item.room_type = strdup(strtok(NULL, ",")); 
   item.price = atof(strtok(NULL, ",")); 
   item.minimum_nights = atoi(strtok(NULL, ",")); 
   item.number_of_reviews = atoi(strtok(NULL, ",")); 
   item.calculated_host_listings_count = atoi(strtok(NULL, ",")); 
   item.availability_365 = atoi(strtok(NULL, ",")); 

   return item; 
} 

void displayStruct(FILE *fp, struct listing temp)
{
    fprintf(fp, "%d, %d, %s, %s, %s, %.6f, %.6f, %s, %.2f, %d, %d, %d, %d\n",
            temp.id, temp.host_id, temp.host_name, temp.neighbourhood_group,
            temp.neighbourhood, temp.latitude, temp.longitude,
            temp.room_type, temp.price, temp.minimum_nights,
            temp.number_of_reviews,
            temp.calculated_host_listings_count,
            temp.availability_365);
}

void swap(struct listing *a, struct listing *b)
{
    struct listing temp = *a;
    *a = *b;
    *b = temp;
}


int partition_hostName(struct listing arr[], int low, int high) //this is for host_name
{
    char *pivot = arr[high].host_name;
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (strcmp(arr[j].host_name, pivot) < 0)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

int partition_price(struct listing arr[], int low, int high)
{
    float pivot = arr[high].price;
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j].price < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort_host_name(struct listing arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition_hostName(arr, low, high);

        quickSort_host_name(arr, low, pi - 1);
        quickSort_host_name(arr, pi + 1, high);
    }
}
void quickSort_price(struct listing arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition_price(arr, low, high);

        quickSort_price(arr, low, pi - 1);
        quickSort_price(arr, pi + 1, high);
    }
}

int main(int argc, char *argv[])
{
    FILE *fptr = fopen("listings.csv", "r");
    FILE *fHost = fopen("sorted_host_name.txt", "w");
    FILE *fprice = fopen("sorted_price.txt", "w");
    if (fptr == NULL) {
        perror("fopen");
        return 1;
    }
    if (fHost == NULL) {
        printf("Error: Could not create or open the file.\n");
        return EXIT_FAILURE;}
    if (fprice == NULL) {
        printf("Error: Could not create or open the file.\n");
        return EXIT_FAILURE;}

    char line[1024];
    fgets(line, sizeof(line), fptr); //skips header
    int count = 0, i;
    struct listing list_items[30000]; //hardcoded for listings.csv
    while (fgets(line, sizeof(line), fptr) != NULL)
    {
        list_items[count++] = getfields(line);
    }

    quickSort_host_name(list_items, 0, count - 1);
    for (i = 0; i < count; i++)
    {
        displayStruct(fHost, list_items[i]);
    }

    quickSort_price(list_items, 0, count - 1);

    for (i = 0; i < count; i++)
    {
        displayStruct(fprice, list_items[i]);
    }

    fclose(fptr);
    fclose(fHost);
    fclose(fprice);
    return 0;
}