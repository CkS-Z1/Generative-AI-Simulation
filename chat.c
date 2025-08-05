#include "chat.h"

// �� find_answer �����м������ƶȼ���
int find_answer(char *input, char *response, char *filename) 
{
    FILE *file;
    char line[MAX_QUESTION_LENGTH + MAX_ANSWER_LENGTH + 2];
    char *question;
    char *allans;
    char *equal_sign;
    char answer[2][MAX_ANSWER_LENGTH];
    int answer_count = 0;
    int random;
    char filtered_input[MAX_QUESTION_LENGTH];
    char filtered_question[MAX_QUESTION_LENGTH];
    const char *no_answer[] = 
    {
        "��������һ���̫����أ��ܻ��ַ�ʽ������",
        "��ѽ�����֪ʶ���һ�ûѧ��...�����ҵ�����",
        "���Ҳ���ؼ��ٸ����㣡��ʱ�����ı�İ�~"
    };
    int index;
    unsigned int seed_val; 
    int contain_condition1, contain_condition2, char_condition;
    char *token;
  
    /* ��ʼ�����б��� */
    file = fopen(filename, "r");
    answer_count = 0;
    seed_val = time(NULL);

    if (file == NULL) 
    {
        strncpy(response,  "��ѽ���ҵ�֪ʶ�������·��...����������һ����", MAX_ANSWER_LENGTH-1);
        response[MAX_ANSWER_LENGTH-1] = '\0';
        return 1;
    }

    filter_particles(input, filtered_input);

    while (fgets(line, sizeof(line), file) )
    {
        line[strcspn(line, "\n")] = '\0';

        if ((equal_sign = strchr(line, '|')) != NULL) 
        {
            *equal_sign = '\0';
            question = line;
            allans = equal_sign + 1;

            trim(question);
            trim(allans);

            filter_particles(question, filtered_question);

            //�����𰸵�answer����
            answer_count = 0; // ���ü�����
            for (token = strtok (allans, "||"); token !=NULL && answer_count <2; token = strtok (NULL, "||"))
            {
                trim (token);
                strncpy (answer[answer_count], token, MAX_ANSWER_LENGTH - 1);
                answer [answer_count][MAX_ANSWER_LENGTH - 1] = '\0';
                answer_count ++;
            }

            if (answer_count > 0)
            {
                srand(seed_val);
                random = rand ()%answer_count;

                //����һ
                if (strstr (input, question) != NULL)
                {
                    strncpy (response, answer[random], MAX_ANSWER_LENGTH - 1);
                    fclose (file);
                    return 1 ;
                }

                //������
                if (strstr (question, input) != NULL )
                {
                    sprintf (response, "���ǲ�����˵%s,%s",question, answer[random]);
                    fclose (file);
                    return 1;
                }
                
                //���Ƽ���
                if (similarity(filtered_input, filtered_question) > 50 ) 
                {   
                    strncpy(response, answer[random], MAX_ANSWER_LENGTH-1);
                    response[MAX_ANSWER_LENGTH-1] = '\0';
                    fclose(file);
                    return 1;
                }
            }

            
        }
    }

    fclose(file);
    
    /* ����δ�ҵ��� */
    srand(seed_val);
    index = rand() % (sizeof(no_answer)/sizeof(no_answer[0]));
    strncpy(response, no_answer[index], MAX_ANSWER_LENGTH-1);
    response[MAX_ANSWER_LENGTH-1] = '\0';

    return 1;
}


//��ϣ����
unsigned short fast_hash(const char *str) 
{
    unsigned short hash = 0xAAAA;
    while (*str) 
    {
        char c = tolower(*str++);  
        hash = (hash << 3) ^ c;    
    }
    return hash;
}

//����ȥ���ַ������˵Ŀո�
void trim(char *str) 
{
    char *end;

    // ȥ����ͷ�Ŀո�
    while (isspace((unsigned char)*str)) 
    {
        str++;
    }

    // ����ַ���ȫ�ǿո�
    if (*str == 0) 
    {
        *str = '\0';
        return;
    }

    // ȥ����β�Ŀո�
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) 
    {
        end--;
    }

    //�����һ����Ч�ַ�����ӿ��ַ�
    *(end + 1) = '\0';
}

//�������ƶ�
int similarity(const char *s1, const char *s2) 
{
    /*---------- һ������������ʼ�� ----------*/
    int len1 = strlen(s1), len2 = strlen(s2);
    int max_len = len1 > len2 ? len1 : len2;
    int match = 0, consecutive = 0; // match����ƥ��� consecutive������ƥ�������
    int i = 0, j = 0, k = 3;        // i/j��˫ָ�� k��ͬ��ʼ�ⴰ�ڴ�С
    int skip_penalty = 0;            // ��ת�ͷ����
    float length_ratio = (float)len1 / len2;
    int skip_i = 0, skip_j = 0;
    int look;
    int base_score;
    FILE *final_file = fopen ("RULES\\final.txt", "r");
    char keyword[50];
    char filtered_s1[MAX_QUESTION_LENGTH], filtered_s2[MAX_QUESTION_LENGTH];

    /*---------- ����˫ָ�뻬���ȽϺ����߼� ----------*/
    while (i < len1 && j < len2) 
    {
        int found_syn = 0;
        char c1 = tolower(s1[i]);
        char c2 = tolower(s2[j]);

        /*-- ͬ��ʼ�⣨1-3�ִ��ڣ�--*/
        for (; k >= 1; k--) 
        { 
            if (i+k <= len1 && j+k <= len2) 
            {
                char w1[4] = {0}, w2[4] = {0};
                strncpy(w1, s1+i, k);
                strncpy(w2, s2+j, k);
                if (are_synonyms(w1, w2)) 
                {
                    match += 4 * k + 2; // ������4/�ַ� + ͬ��ʽ���2
                    i += k;            // ������ƥ�䲿��
                    j += k;
                    found_syn = 1;
                    break;
                }
            }
        }
        k = 1; // ���ô��ڴ�С
        if (found_syn) continue;

        /*-- ֱ���ַ�ƥ�� --*/
        if (c1 == c2) 
        {
            match += 4;           // ������4/�ַ�
            consecutive++;        // ����ƥ�����
            i++; j++;
        } 
        else 
        {
            /*-- ����ƥ�佱������2�ַ�ʱ������--*/
            if (consecutive >= 2) 
            {
                match += consecutive * 1; // ���⽱��1/�����ַ�
            }
            consecutive = 0;

            /*-- ������ת���ԣ�ǰհ3�ַ���--*/
            for (look = 1; look <= 3; look++) 
            {
                if (i+look < len1 && s1[i+look] == c2) skip_i = look;
                if (j+look < len2 && s2[j+look] == c1) skip_j = look;
            }
            
            if (skip_i > skip_j) i += skip_i;
            else if (skip_j > 0) j += skip_j;
            else { i++; j++; }    // Ĭ��ͬ��ǰ��
            
            skip_penalty = 1;     // ������ת�ͷ�
            match -= skip_penalty;// �۳���ת��
        }
    }

    /*-- ��������ƥ�佱�� --*/
    if (consecutive >= 2) 
    {
        match += consecutive * 3;
    }

    /*---------- �����������ƶȼ��� ----------*/
    base_score = (match * 100) / (max_len * 4);

    /*---------- �ġ��ַ�������ϵ�ӳ� ----------*/
    if (check_sentences(s1, s2)) 
    {
        /* ����������һ����ʱ������
           1. s1���ַ���s2���Ӽ���������ʺ�
           2. s2���ַ���s1���Ӽ�
           �ӳ��߼���
           - ��������<40��ǿ��أ�����40��
           - �������֡�40��+15% ����ƥ�� */
        if (base_score < 40) 
        {
            base_score = 40;
        } 
        else 
        {
            base_score += 15;
            if (base_score > 100) 
            {
                base_score = 100;
            }
        }
    }
    /*---------- �塢final��txt�ؼ��ʶ�̬ƥ��ӳ� ----------*/
    if (final_file)
    {
        filter_particles (s1, filtered_s1);
        filter_particles (s2, filtered_s2);

        while (fgets (keyword, sizeof (keyword), final_file))
        {
            keyword[strcspn (keyword, "\n")] = '\0';
            trim(keyword);
            if (strlen (keyword) == 0)
            {
                continue;
            }
            if (strstr (filtered_s1, keyword)||strstr (filtered_s2, keyword))
            {
                base_score += 20;
                if (base_score > 100)
                {
                    base_score = 100;
                }
                break;
            }
        }
        fclose (final_file);
    }
    return base_score;
}

// ����ַ�������ϵ�ĺ���
int is_subset(const char *a, const char *b) 
{
    unsigned short hash[256] = {0};
    for (; *b; b++) hash[(unsigned char)*b] = 1;
    for (; *a; a++) if (!hash[(unsigned char)*a]) return 0;
    return 1;
}

// ˫�����ַ�����
int check_sentences(const char *a, const char *b) 
{
    return is_subset(a, b) || is_subset(b, a);
}

//����ԱȺ���
int are_synonyms(const char *w1, const char *w2) 
{
    int found = 0;
    char buf[8] = {0}; 
    int idx = 0, has_w1 = 0, has_w2 = 0;
    int c = 0;
    const char *filename = "C:\\PRJ\\WORDS\\synonyms.txt";

    FILE *file = fopen(filename, "r");
    if (!file) return 0;

    while ((c = fgetc(file)) != EOF) 
    {
        if (c == ',' || c == '??' || c == '\n') 
        {
            buf[idx] = '\0';
            // ��̬�ȶԵ�ǰ�ʶ�
            if (strcmp(buf, w1) == 0) has_w1 = 1;
            if (strcmp(buf, w2) == 0) has_w2 = 1;
            idx = 0;

            if (c == '\n') 
            {  // �н������
                if (has_w1 && has_w2) found = 1;
                else has_w1 = has_w2 = 0;
                if (found) break;
            }
        } 
        else if (idx < 7) 
        {
            buf[idx++] = c; 
        }
    }
    fclose(file);
    return found;
}

// 
void filter_particles(const char *input, char *output) 
{
    const char **particle = PARTICLES;
    char temp[MAX_QUESTION_LENGTH];
    strncpy(temp, input, MAX_QUESTION_LENGTH - 1);
    temp[MAX_QUESTION_LENGTH - 1] = '\0';
    
     // ȥ���������
    for (particle = PARTICLES; *particle != NULL; particle++) 
    {
        char *pos = strstr(temp, *particle);
        while (pos != NULL) 
        {
             // �ƶ��ַ����������
            memmove(pos, pos + strlen(*particle), strlen(pos + strlen(*particle)) + 1);
            pos = strstr(temp, *particle);
        }
    }
    
    strncpy(output, temp, MAX_QUESTION_LENGTH - 1);
    output[MAX_QUESTION_LENGTH - 1] = '\0';
}
