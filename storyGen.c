/* storyGen.c */
#include "storyGen.h"

// 故事生成器核心函数
int generateStory(storyInput input)
{
    static char path[MAX_PATH];      // 静态缓冲区，复用路径存储
    static char buffer[MAX_CONTENT]; // 静态行缓冲区
    static char isEnd[10] = {0};
    char *ending_type;
    FILE *fp;
    int story_num = rand() % 10;
    int score = 0;
    int i;

    srand(time(NULL));

    // 加载故事开头
    sprintf(path, "%sSTART\\%c\\%d\\START.TXT", STORY_ROOT, input.genre, story_num);
    if ((fp = fopen(path, "r")) != NULL)
    {
        while (fgets(buffer, sizeof(buffer), fp))
        {
            replacePlaceholder(buffer, input);
            outputText(buffer, 1);
        }
        fclose(fp);
    }

    // 故事情节处理
    for (i = 0; i < 5 && score < 20 && score > -10; i++)
    {
        int targetPlot, currentPlot = 0;
        char type;
        char *plotTag;
        char *dirTag;

        // 构建文件路径
        type = rand() % 2 ? 'A' : 'C';
        plotTag = (type == 'A') ? "<achieve>" : "<crisis>";
        dirTag = (type == 'A') ? "achieve" : "crisis";

        // 构建文件路径
        sprintf(path, "%sPLOT\\%c\\%s\\%s\\%d\\plot.txt",
                STORY_ROOT, input.genre, dirTag, input.personality, story_num);

        // 随机生成情节
        targetPlot = rand() % 20;
        if ((fp = fopen(path, "r")) != NULL)
        {
            int inTargetPlot = 0;
            int branchStarted = 0;
            static char branchPart[MAX_CONTENT] = {0}; // 静态分支缓冲区
            memset(branchPart, 0, sizeof(branchPart));
            while (fgets(buffer, sizeof(buffer), fp))
            {
                // 定位目标情节
                if (!inTargetPlot)
                {
                    if (strstr(buffer, plotTag))
                    {
                        if (currentPlot == targetPlot)
                        {
                            inTargetPlot = 1;
                        }
                        else
                        {
                            currentPlot++;
                            continue;
                        }
                    }
                    continue;
                }
                if (buffer[0] == '\r' || buffer[0] == '\n')
                {
                    continue;
                }
                // 处理目标情节内容
                // 如果分支情节没有开始，证明还在爬取主干情节
                if (!branchStarted)
                {
                    char *branchStart = strchr(buffer, '[');
                    if (branchStart)
                    {
                        char *remaining;
                        *branchStart = '\0';
                        replacePlaceholder(buffer, input);
                        buffer[strcspn(buffer, "\r\n")] = 0;
                        outputText(buffer, 1);

                        branchStarted = 1;
                        branchPart[0] = '\0';
                        remaining = branchStart + 1;
                        processBranch(remaining, input, &score, type, branchPart);
                    }
                    else
                    {
                        replacePlaceholder(buffer, input);
                        buffer[strcspn(buffer, "\r\n")] = 0;
                        outputText(buffer, 1);
                    }
                }
                else
                {
                    processBranch(buffer, input, &score, type, branchPart);
                }

                // 提前退出条件
                if (score >= 30 || score <= -10)
                {
                    break;
                }
            }
            fclose(fp);
        }
    }

    // 处理结局（与原逻辑相同，但path复用）
    ending_type = score > 20 ? "good" : (score >= 0 ? "ordinary" : "bad");
    sprintf(path, "%sENDING\\%c\\%d\\%s\\ending.txt",
            STORY_ROOT, input.genre, story_num, ending_type);

    if ((fp = fopen(path, "r")) != NULL)
    {
        int target_ending = rand() % 10; // 每个结局文件有10条结局
        int current_ending = 0;
        char ending_tag[20];
        sprintf(ending_tag, "<%s_ending>", ending_type); // 例如生成 "<bad_ending>"
        while (fgets(buffer, MAX_CONTENT, fp))
        {
            if (strstr(buffer, ending_tag))
            { // 检测 <%s_ending>
                if (current_ending == target_ending)
                {
                    // 找到目标结局，读取直到下一个 <%s_ending> 或文件结束
                    while (fgets(buffer, MAX_CONTENT, fp))
                    {
                        if (strstr(buffer, ending_tag))
                        {
                            break; // 结局以 <ending> 分隔,读到下一个时，说明读取结束了
                        }
                        replacePlaceholder(buffer, input); // 替换占位符
                        replaceTag(buffer, "<good_ending>", "");
                        replaceTag(buffer, "<bad_ending>", "");
                        replaceTag(buffer, "<ordinary_ending>", "");
                        outputText(buffer, 1);
                    }
                    break;
                }
                current_ending++;
            }
        }
        fclose(fp);
    }

    outputText(isEnd, 2);
    return story_num;
}

// 增加了概率链 / 马尔科夫链
void processBranch(char *str, storyInput input, int *score, char type, char *branchPart)
{
    char *endBracket = strchr(str, ']');
    char *option;
    if (*str == '[')
    {
        str++;
    }
    if (endBracket)
    {
        *endBracket = '\0';
    }
    str[strcspn(str, "\r\n")] = 0;
    strncat(branchPart, str, sizeof(branchPart) - strlen(branchPart) - 1);
    option = strtok(branchPart, "|");

    if (endBracket)
    {
        while (option)
        {
            int delta;
            char *buffer;
            if (type == 'A')
            {
                delta = abs(strtol(option, &buffer, 10));
            }
            else
            {
                delta = strtol(option, &buffer, 10);
            }
            if (rand() % 100 < 33)
            {
                *score += delta;
                // 去除占位符
                replaceTag(buffer, "<cr", "");
                replaceTag(buffer, "<ac", "");
                replaceTag(buffer, "<", "");
                if (buffer[0] == '\r' || buffer[0] == '\n')
                {
                    break;
                }
                buffer[strcspn(buffer, "\r\n")] = 0;
                break;
            }
            option = strtok(NULL, "|");
        }
        branchPart[0] = '\0';
    }
}

// 占位符替换函数
void replacePlaceholder(char *str, storyInput input)
{
    static char path[MAX_PATH] = {0};
    FILE *fp = NULL;
    char buffer[MAX_CONTENT] = {0};
    char cleanBuffer[MAX_CONTENT] = {0};
    int i, target_line = 0;

    // 替换核心标签
    replaceTag(str, "<main_name>", input.mainName);
    replaceTag(str, "<main_location>", input.location);
    replaceTag(str, "<personality>", input.personality);
    replaceTag(str, "ta", input.gender ? "她" : "他");
    replaceTag(str, "gender", input.gender ? "女" : "男");

    // 处理外貌描述
    if (strstr(str, "<look>"))
    {
        sprintf(path, "STORY\\look\\%d\\look.txt", input.gender);
        fp = fopen(path, "r");
        if (fp)
        {
            // 随机读取并清洗标签
            target_line = rand() % 100 + 1;
            for (i = 1; fgets(buffer, MAX_CONTENT, fp); i++)
            {
                if (i == target_line)
                {
                    extractContent(buffer, "<look>", "\n", cleanBuffer);
                    replaceTag(str, "<look>", cleanBuffer);
                    break;
                }
            }
            fclose(fp);
        }
    }

    // // 处理配角信息
    if (strstr(str, "<name>") || strstr(str, "<name_describe>"))
    {
        int name_gender;
        name_gender = rand() % 2;
        sprintf(path, "STORY\\name\\%c\\%d\\name.txt", input.genre, name_gender);
        fp = fopen(path, "r");
        if (fp)
        {
            // 读取并解析复合标签
            int max_line;
            max_line = (name_gender == 0) ? 100 : 60;
            target_line = rand() % max_line + 1;
            delay(100);

            for (i = 1; fgets(buffer, MAX_CONTENT, fp); i++)
            {
                if (i == target_line)
                {
                    // 解析形如<name>林清羽<name_describe>天体物理学家...
                    char name[64] = {0};
                    char describe[256] = {0};
                    extractContent(buffer, "<name>", "<name_describe>", name);
                    extractContent(buffer, "<name_describe>", "\n", describe);
                    replaceTag(str, "<name>", name);
                    replaceTag(str, "<name_describe>", describe);
                    break;
                }
            }
            fclose(fp);
        }
        else
        {
            replaceTag(str, "<name>", "某人");
            replaceTag(str, "<name_describe>", "");
            // break;
        }
    }

    // 处理<location>,<pos_describe>
    if (strstr(str, "<location>") || strstr(str, "<pos_describe>"))
    {
        sprintf(path, "STORY\\location\\%c\\location.txt", input.genre);
        fp = fopen(path, "r");
        if (fp)
        {
            // 读取并解析复合标签
            target_line = rand() % 50 + 1;
            delay(100);

            for (i = 1; fgets(buffer, MAX_CONTENT, fp); i++)
            {
                if (i == target_line)
                {
                    // 解析形如<location>空间站<pos_describe>空间中的一抹尘埃...
                    char pos[64] = {0};
                    char describe[256] = {0};
                    extractContent(buffer, "<location>", "<pos_describe>", pos);
                    extractContent(buffer, "<pos_describe>", "\n", describe);

                    replaceTag(str, "<location>", pos);
                    replaceTag(str, "<pos_describe>", describe);
                    break;
                }
            }
            fclose(fp);
        }
        else
        {
            replaceTag(str, "<name>", "某地");
            replaceTag(str, "<name_describe>", "");
        }
    }

    // 处理话语类占位符
    while (strstr(str, "<words_good>") || strstr(str, "<words_bad>") ||
           strstr(str, "<think_good>") || strstr(str, "<think_bad>"))
    {
        // 构建words路径
        sprintf(path, "STORY\\words\\%s\\words.txt", input.personality);
        fp = fopen(path, "r");
        if (fp)
        {
            // 确定行数范围
            int start_line = 1, end_line = 100;
            if (strstr(str, "<words_good>"))
            {
                if (strcmp(input.personality, "01") == 0)
                {
                    start_line = 1;
                    end_line = 30;
                }
                else
                {
                    start_line = 1;
                    end_line = 60;
                }
            }
            else if (strstr(str, "<words_bad>"))
            {
                if (strcmp(input.personality, "01") == 0)
                {
                    start_line = 61;
                    end_line = 80;
                }
                else
                {
                    start_line = 61;
                    end_line = 100;
                }
            }
            else if (strstr(str, "<think_good>"))
            {
                start_line = 31;
                end_line = 60;
            }
            else if (strstr(str, "<think_bad>"))
            {
                start_line = 81;
                end_line = 100;
            }

            target_line = rand() % (end_line - start_line + 1) + start_line;
            delay(100);

            for (i = 1; fgets(buffer, MAX_CONTENT, fp); i++)
            {
                if (i == target_line)
                {
                    // extractContent(buffer, "<look>", "\n", cleanBuffer);
                    // replaceTag(str, "<look>", cleanBuffer);
                    if (strstr(str, "<words_good>"))
                    {
                        extractContent(buffer, "<words_good>", "\n", cleanBuffer);
                        replaceTag(str, "<words_good>", cleanBuffer);
                    }
                    else if (strstr(str, "<words_bad>"))
                    {
                        extractContent(buffer, "<words_bad>", "\n", cleanBuffer);
                        replaceTag(str, "<words_bad>", cleanBuffer);
                    }
                    else if (strstr(str, "<think_good>"))
                    {
                        extractContent(buffer, "<think_good>", "\n", cleanBuffer);
                        replaceTag(str, "<think_good>", cleanBuffer);
                    }
                    else if (strstr(str, "<think_bad>"))
                    {
                        extractContent(buffer, "<think_bad>", "\n", cleanBuffer);
                        replaceTag(str, "<think_bad>", cleanBuffer);
                    }
                    break;
                }
            }
            fclose(fp);
        }
        else
        {
            // 文件打开失败，替换默认值
            replaceTag(str, "<words_good>", "\"加油！\"");
            replaceTag(str, "<words_bad>", "\"糟糕！\"");
            break;
        }
    }
}

// 通用标签内容提取函数
void extractContent(const char *source, const char *startTag, const char *endTag, char *output)
{
    char *start, *end;
    size_t len;

    start = strstr(source, startTag);
    if (!start)
    {
        strncpy(output, source, MAX_CONTENT - 1);
        output[MAX_CONTENT - 1] = 0;
        return;
    }
    start += strlen(startTag);

    end = strstr(start, endTag);
    if (!end)
    {
        end = start + strlen(start);
    }

    len = end - start;
    strncpy(output, start, len);
    output[len] = '\0';

    // 二次过滤换行符
    output[strcspn(output, "\r\n")] = 0;
}

// 标签替换函数
void replaceTag(char *dest, const char *tag, const char *value)
{
    static char temp[MAX_CONTENT * 2] = {0}; // 双倍缓冲防止溢出
    char *pos = NULL;
    int tagLen = strlen(tag);
    int valLen = strlen(value);

    while ((pos = strstr(dest, tag)) != NULL)
    {
        // 计算替换后长度
        size_t frontLen = pos - dest;
        size_t totalLen = frontLen + valLen + strlen(pos + tagLen);

        if (totalLen >= MAX_CONTENT)
        {
            break; // 防止缓冲区溢出
        }

        // 构建临时字符串
        strncpy(temp, dest, frontLen);
        temp[frontLen] = '\0'; // 确保终止
        strncat(temp, value, sizeof(temp) - strlen(temp) - 1);
        strncat(temp, pos + tagLen, sizeof(temp) - strlen(temp) - 1);

        // 写回原缓冲区
        strncpy(dest, temp, MAX_CONTENT - 1);
        dest[MAX_CONTENT - 1] = '\0';

        // 移动指针继续查找
        pos = dest + frontLen + valLen;
    }
}

// 故事生成函数
int storyGen(storyInput input)
{
    int story_num;
    srand(time(NULL));
    reset_cursor();
    // 生成故事
    story_num = generateStory(input);
    cleardevice();
    show_story();

    return story_num;
}