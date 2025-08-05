#include "chat.h"

// 在 find_answer 函数中集成相似度计算
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
        "这个问题我还不太清楚呢，能换种方式问我吗？",
        "哎呀，这个知识点我还没学会...能问我点别的吗？",
        "等我查查秘籍再告诉你！暂时先聊聊别的吧~"
    };
    int index;
    unsigned int seed_val; 
    int contain_condition1, contain_condition2, char_condition;
    char *token;
  
    /* 初始化所有变量 */
    file = fopen(filename, "r");
    answer_count = 0;
    seed_val = time(NULL);

    if (file == NULL) 
    {
        strncpy(response,  "哎呀，我的知识库好像迷路了...你能再问我一次吗？", MAX_ANSWER_LENGTH-1);
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

            //解析答案到answer数组
            answer_count = 0; // 重置计数器
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

                //包含一
                if (strstr (input, question) != NULL)
                {
                    strncpy (response, answer[random], MAX_ANSWER_LENGTH - 1);
                    fclose (file);
                    return 1 ;
                }

                //包含二
                if (strstr (question, input) != NULL )
                {
                    sprintf (response, "你是不是想说%s,%s",question, answer[random]);
                    fclose (file);
                    return 1;
                }
                
                //相似计算
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
    
    /* 处理未找到答案 */
    srand(seed_val);
    index = rand() % (sizeof(no_answer)/sizeof(no_answer[0]));
    strncpy(response, no_answer[index], MAX_ANSWER_LENGTH-1);
    response[MAX_ANSWER_LENGTH-1] = '\0';

    return 1;
}


//哈希函数
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

//用于去除字符串两端的空格
void trim(char *str) 
{
    char *end;

    // 去掉开头的空格
    while (isspace((unsigned char)*str)) 
    {
        str++;
    }

    // 如果字符串全是空格
    if (*str == 0) 
    {
        *str = '\0';
        return;
    }

    // 去掉结尾的空格
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) 
    {
        end--;
    }

    //在最后一个有效字符后添加空字符
    *(end + 1) = '\0';
}

//计算相似度
int similarity(const char *s1, const char *s2) 
{
    /*---------- 一、基础参数初始化 ----------*/
    int len1 = strlen(s1), len2 = strlen(s2);
    int max_len = len1 > len2 ? len1 : len2;
    int match = 0, consecutive = 0; // match：总匹配分 consecutive：连续匹配计数器
    int i = 0, j = 0, k = 3;        // i/j：双指针 k：同义词检测窗口大小
    int skip_penalty = 0;            // 跳转惩罚标记
    float length_ratio = (float)len1 / len2;
    int skip_i = 0, skip_j = 0;
    int look;
    int base_score;
    FILE *final_file = fopen ("RULES\\final.txt", "r");
    char keyword[50];
    char filtered_s1[MAX_QUESTION_LENGTH], filtered_s2[MAX_QUESTION_LENGTH];

    /*---------- 二、双指针滑动比较核心逻辑 ----------*/
    while (i < len1 && j < len2) 
    {
        int found_syn = 0;
        char c1 = tolower(s1[i]);
        char c2 = tolower(s2[j]);

        /*-- 同义词检测（1-3字窗口）--*/
        for (; k >= 1; k--) 
        { 
            if (i+k <= len1 && j+k <= len2) 
            {
                char w1[4] = {0}, w2[4] = {0};
                strncpy(w1, s1+i, k);
                strncpy(w2, s2+j, k);
                if (are_synonyms(w1, w2)) 
                {
                    match += 4 * k + 2; // 基础分4/字符 + 同义词奖励2
                    i += k;            // 跳过已匹配部分
                    j += k;
                    found_syn = 1;
                    break;
                }
            }
        }
        k = 1; // 重置窗口大小
        if (found_syn) continue;

        /*-- 直接字符匹配 --*/
        if (c1 == c2) 
        {
            match += 4;           // 基础分4/字符
            consecutive++;        // 连续匹配计数
            i++; j++;
        } 
        else 
        {
            /*-- 连续匹配奖励（≥2字符时触发）--*/
            if (consecutive >= 2) 
            {
                match += consecutive * 1; // 额外奖励1/连续字符
            }
            consecutive = 0;

            /*-- 智能跳转策略（前瞻3字符）--*/
            for (look = 1; look <= 3; look++) 
            {
                if (i+look < len1 && s1[i+look] == c2) skip_i = look;
                if (j+look < len2 && s2[j+look] == c1) skip_j = look;
            }
            
            if (skip_i > skip_j) i += skip_i;
            else if (skip_j > 0) j += skip_j;
            else { i++; j++; }    // 默认同步前进
            
            skip_penalty = 1;     // 触发跳转惩罚
            match -= skip_penalty;// 扣除跳转分
        }
    }

    /*-- 最终连续匹配奖励 --*/
    if (consecutive >= 2) 
    {
        match += consecutive * 3;
    }

    /*---------- 三、基础相似度计算 ----------*/
    base_score = (match * 100) / (max_len * 4);

    /*---------- 四、字符包含关系加成 ----------*/
    if (check_sentences(s1, s2)) 
    {
        /* 满足以下任一条件时触发：
           1. s1的字符是s2的子集（过滤虚词后）
           2. s2的字符是s1的子集
           加成逻辑：
           - 若基础分<40：强相关，保底40分
           - 若基础分≥40：+15% 鼓励匹配 */
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
    /*---------- 五、final。txt关键词动态匹配加成 ----------*/
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

// 检查字符包含关系的函数
int is_subset(const char *a, const char *b) 
{
    unsigned short hash[256] = {0};
    for (; *b; b++) hash[(unsigned char)*b] = 1;
    for (; *a; a++) if (!hash[(unsigned char)*a]) return 0;
    return 1;
}

// 双向检查字符包含
int check_sentences(const char *a, const char *b) 
{
    return is_subset(a, b) || is_subset(b, a);
}

//词义对比函数
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
            // 动态比对当前词段
            if (strcmp(buf, w1) == 0) has_w1 = 1;
            if (strcmp(buf, w2) == 0) has_w2 = 1;
            idx = 0;

            if (c == '\n') 
            {  // 行结束检查
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
    
     // 去除所有虚词
    for (particle = PARTICLES; *particle != NULL; particle++) 
    {
        char *pos = strstr(temp, *particle);
        while (pos != NULL) 
        {
             // 移动字符串覆盖虚词
            memmove(pos, pos + strlen(*particle), strlen(pos + strlen(*particle)) + 1);
            pos = strstr(temp, *particle);
        }
    }
    
    strncpy(output, temp, MAX_QUESTION_LENGTH - 1);
    output[MAX_QUESTION_LENGTH - 1] = '\0';
}
