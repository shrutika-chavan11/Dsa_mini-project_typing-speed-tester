#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SESSIONS 10

typedef struct Session {
    int sessionID;               // Unique session number
    double wpm;
    double accuracy;
    char timestamp[50];
    struct Session *prev;
    struct Session *next;
} Session;

Session *head = NULL;
Session *tail = NULL;
Session *current = NULL;

int sessionCount = 0;
int globalSessionID = 0;   // Session counter

// Create timestamp
void makeTimestamp(char *buf)
{
    time_t t = time(NULL);
    struct tm *tmInfo = localtime(&t);
    strftime(buf, 50, "%Y-%m-%d %H:%M:%S", tmInfo);
}

// Create new session node
Session* create_node(double wpm, double acc)
{
    Session *s = malloc(sizeof(Session));

    s->sessionID = ++globalSessionID;   // Auto-increment session ID
    s->wpm = wpm;
    s->accuracy = acc;
    makeTimestamp(s->timestamp);

    s->prev = NULL;
    s->next = NULL;
    return s;
}

// Remove oldest (head)
void remove_oldest()
{
    if (!head) return;

    Session *tmp = head;
    head = head->next;

    if (head)
        head->prev = NULL;
    else
        tail = NULL;

    Session *temp = head;
    while(temp) {
        (temp->sessionID)--;
        temp = temp->next;
    }
    free(tmp);
    sessionCount--;
}

// Add session to list
void add_session(double wpm, double accuracy)
{
    Session *node = create_node(wpm, accuracy);

    if (!head)
        head = tail = node;
    else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }

    sessionCount++;

    while (sessionCount > MAX_SESSIONS)
        remove_oldest();

    current = tail;   // Reset navigation to latest session
}

// Display all sessions
void displayAllSessions()
{
    if (!head) {
        printf("\nNo sessions recorded yet.\n");
        return;
    }

    printf("\n--- Session History (Oldest → Newest) ---\n");

    Session *cur = head;

    while (cur) {
        printf("Session #%d | WPM: %.2f | ACC: %.2f%%\n", 
               cur->sessionID, cur->wpm, cur->accuracy);
        printf("Time: %s\n\n", cur->timestamp);
        cur = cur->next;
    }
}

// Show previous session
void displayPrevSession()
{
    if (!current) {
        printf("No session data.\n");
        return;
    }

    if (current->prev)
        current = current->prev;

    printf("\nSession #%d\nWPM: %.2f | ACC: %.2f%%\nTime: %s\n", current->sessionID, current->wpm, current->accuracy, current->timestamp);
}

// Show next session
void displayNextSession()
{
    if (!current) {
        printf("No session data.\n");
        return;
    }

    if (current->next)
        current = current->next;

    printf("\nSession #%d\nWPM: %.2f | ACC: %.2f%%\nTime: %s\n",
        current->sessionID, current->wpm, current->accuracy, current->timestamp);
}

// For keeping graph numbers separate
int get_graph_no()
{
    FILE *f = fopen("graph_no.txt", "r");
    int n = 0;

    if (f) {
        fscanf(f, "%d", &n);
        fclose(f);
    }

    n++;

    f = fopen("graph_no.txt", "w");
    fprintf(f, "%d", n);
    fclose(f);

    return n;
}

// Plot graph using Gnuplot
void plotGraph()
{
    if (!head) {
        printf("No sessions to plot.\n");
        return;
    }

    FILE *data = fopen("wpm_data.txt", "w");
    int i = 1;
    Session *cur = head;

    while (cur) {
        fprintf(data, "%d %.2f\n", i++, cur->wpm);
        cur = cur->next;
    }
    fclose(data);

    int gno = get_graph_no();
    char imgName[50];
    sprintf(imgName, "wpm_graph_%d.png", gno);

    FILE *gp = fopen("plot_script.gp", "w");
    fprintf(gp, "set terminal png size 1000,600\n");
    fprintf(gp, "set output '%s'\n", imgName);
    fprintf(gp, "set title 'WPM Progress (Session 1 to %d)'\n", globalSessionID);
    fprintf(gp, "set xlabel 'Session Number'\n");
    fprintf(gp, "set ylabel 'WPM'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "plot 'wpm_data.txt' using 1:2 with linespoints title 'WPM'\n");
    fclose(gp);

    system("gnuplot plot_script.gp");

    printf("Graph saved as %s\n", imgName);
    char openCmd[100];
    sprintf(openCmd, "xdg-open %s &", imgName);
    system(openCmd);

}

// Plot Accuracy graph using Gnuplot
void plotAccuracyGraph()
{
    if (!head) {
        printf("No sessions to plot.\n");
        return;
    }

    FILE *data = fopen("acc_data.txt", "w");
    int i = 1;
    Session *cur = head;

    while (cur) {
        fprintf(data, "%d %.2f\n", i++, cur->accuracy);
        cur = cur->next;
    }
    fclose(data);

    int gno = get_graph_no();
    char imgName[50];
    sprintf(imgName, "acc_graph_%d.png", gno);

    FILE *gp = fopen("plot_acc_script.gp", "w");
    fprintf(gp, "set terminal png size 1000,600\n");
    fprintf(gp, "set output '%s'\n", imgName);
    fprintf(gp, "set title 'Accuracy Progress (Session 1 to %d)'\n", globalSessionID);
    fprintf(gp, "set xlabel 'Session Number'\n");
    fprintf(gp, "set ylabel 'Accuracy (%%)'\n");
    fprintf(gp, "set yrange [0:100]\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "plot 'acc_data.txt' using 1:2 with linespoints title 'Accuracy'\n");
    fclose(gp);

    system("gnuplot plot_acc_script.gp");
    char openCmd[100];
    sprintf(openCmd, "xdg-open %s &", imgName);
    system(openCmd);

    printf("Accuracy graph saved as %s\n", imgName);
}

void save_text_files(void)
{
    FILE *data  = fopen("wpm_data.txt", "w");
    FILE *data1 = fopen("acc_data.txt", "w");
    FILE *data2 = fopen("time_stamp.txt", "w");

    if (!data || !data1 || !data2) {
        perror("Error opening one of the session data files for writing");
        if (data)  fclose(data);
        if (data1) fclose(data1);
        if (data2) fclose(data2);
        return;
    }

    Session *cur = head;

    while (cur) {
        int id = cur->sessionID;

        fprintf(data,  "%d %.2f\n", id, cur->wpm);
        fprintf(data1, "%d %.2f\n", id, cur->accuracy);
        fprintf(data2, "%d %s\n",  id, cur->timestamp);

        cur = cur->next;
    }

    fclose(data);
    fclose(data1);
    fclose(data2);
}

void load_sessions_from_files(void)
{
    FILE *fw = fopen("wpm_data.txt", "r");
    FILE *fa = fopen("acc_data.txt", "r");
    FILE *ft = fopen("time_stamp.txt", "r");

    if (!fw || !fa || !ft) {
        printf("Could not open one or more session data files for reading.\n");
        if (fw) fclose(fw);
        if (fa) fclose(fa);
        if (ft) fclose(ft);
        return;
    }

    int id_w, id_a, id_t;
    double wpm, acc;
    char timestamp[50];

    while (fscanf(fw, "%d %lf", &id_w, &wpm) == 2 &&
           fscanf(fa, "%d %lf", &id_a, &acc) == 2 &&
           fscanf(ft, "%d %49[^\n]", &id_t, timestamp) == 2) {

        // Optional safety check:
        // if (id_w != id_a || id_w != id_t) {
        //     printf("Session ID mismatch in files: %d, %d, %d\n", id_w, id_a, id_t);
        //     break;
        // }

        Session *node = malloc(sizeof(Session));
        if (!node) {
            perror("malloc failed in load_sessions_from_files");
            break;
        }

        node->sessionID = id_w;
        node->wpm       = wpm;
        node->accuracy  = acc;

        strncpy(node->timestamp, timestamp, sizeof(node->timestamp));
        node->timestamp[sizeof(node->timestamp) - 1] = '\0';

        node->prev = tail;
        node->next = NULL;

        if (!head) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }

        sessionCount++;
    }

    fclose(fw);
    fclose(fa);
    fclose(ft);

    // Make sure new sessions continue from the last loaded ID
    if (tail)
        globalSessionID = tail->sessionID;
}
