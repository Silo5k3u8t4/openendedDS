#include <stdio.h>

int main() {
    // Open the CSV file for writing
    FILE *csvFile = fopen("spotify_tracks.csv", "w");
    if (!csvFile) {
        printf("Error creating the CSV file.\n");
        return 1;
    }

    // Write sample Spotify track links to the file
    fprintf(csvFile, "https://open.spotify.com/track/3xMHXmedL5Rvfxmiar9Ryv?si=776b45e852654dc6\n");
    fprintf(csvFile, "https://open.spotify.com/track/2h5UfY3Mt4fkbysxYkhGpG?si=28016ceb6bc94749\n");
    fprintf(csvFile, "https://open.spotify.com/track/2zVOFcdPAurQMbP1D6czwo?si=d1e3b41d33164795\n");
    fprintf(csvFile, "https://open.spotify.com/track/1B02UI29t3PTh3m98absaP?si=d0d2656f182448b9\n");
    // Close the file
    fclose(csvFile);

    printf("CSV file 'spotify_tracks.csv' created successfully.\n");

    return 0;
}
