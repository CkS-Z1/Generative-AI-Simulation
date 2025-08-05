/* storyGen.c */
#include "storyGen.h"

// �������������ĺ���
int generateStory(storyInput input)
{
    static char path[MAX_PATH];      // ��̬������������·���洢
    static char buffer[MAX_CONTENT]; // ��̬�л�����
    static char isEnd[10] = {0};
    char *ending_type;
    FILE *fp;
    int story_num = rand() % 10;
    int score = 0;
    int i;

    srand(time(NULL));

    // ���ع��¿�ͷ
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

    // ������ڴ���
    for (i = 0; i < 5 && score < 20 && score > -10; i++)
    {
        int targetPlot, currentPlot = 0;
        char type;
        char *plotTag;
        char *dirTag;

        // �����ļ�·��
        type = rand() % 2 ? 'A' : 'C';
        plotTag = (type == 'A') ? "<achieve>" : "<crisis>";
        dirTag = (type == 'A') ? "achieve" : "crisis";

        // �����ļ�·��
        sprintf(path, "%sPLOT\\%c\\%s\\%s\\%d\\plot.txt",
                STORY_ROOT, input.genre, dirTag, input.personality, story_num);

        // ����������
        targetPlot = rand() % 20;
        if ((fp = fopen(path, "r")) != NULL)
        {
            int inTargetPlot = 0;
            int branchStarted = 0;
            static char branchPart[MAX_CONTENT] = {0}; // ��̬��֧������
            memset(branchPart, 0, sizeof(branchPart));
            while (fgets(buffer, sizeof(buffer), fp))
            {
                // ��λĿ�����
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
                // ����Ŀ���������
                // �����֧���û�п�ʼ��֤��������ȡ�������
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

                // ��ǰ�˳�����
                if (score >= 30 || score <= -10)
                {
                    break;
                }
            }
            fclose(fp);
        }
    }

    // �����֣���ԭ�߼���ͬ����path���ã�
    ending_type = score > 20 ? "good" : (score >= 0 ? "ordinary" : "bad");
    sprintf(path, "%sENDING\\%c\\%d\\%s\\ending.txt",
            STORY_ROOT, input.genre, story_num, ending_type);

    if ((fp = fopen(path, "r")) != NULL)
    {
        int target_ending = rand() % 10; // ÿ������ļ���10�����
        int current_ending = 0;
        char ending_tag[20];
        sprintf(ending_tag, "<%s_ending>", ending_type); // �������� "<bad_ending>"
        while (fgets(buffer, MAX_CONTENT, fp))
        {
            if (strstr(buffer, ending_tag))
            { // ��� <%s_ending>
                if (current_ending == target_ending)
                {
                    // �ҵ�Ŀ���֣���ȡֱ����һ�� <%s_ending> ���ļ�����
                    while (fgets(buffer, MAX_CONTENT, fp))
                    {
                        if (strstr(buffer, ending_tag))
                        {
                            break; // ����� <ending> �ָ�,������һ��ʱ��˵����ȡ������
                        }
                        replacePlaceholder(buffer, input); // �滻ռλ��
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

// �����˸����� / ����Ʒ���
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
                // ȥ��ռλ��
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

// ռλ���滻����
void replacePlaceholder(char *str, storyInput input)
{
    static char path[MAX_PATH] = {0};
    FILE *fp = NULL;
    char buffer[MAX_CONTENT] = {0};
    char cleanBuffer[MAX_CONTENT] = {0};
    int i, target_line = 0;

    // �滻���ı�ǩ
    replaceTag(str, "<main_name>", input.mainName);
    replaceTag(str, "<main_location>", input.location);
    replaceTag(str, "<personality>", input.personality);
    replaceTag(str, "ta", input.gender ? "��" : "��");
    replaceTag(str, "gender", input.gender ? "Ů" : "��");

    // ������ò����
    if (strstr(str, "<look>"))
    {
        sprintf(path, "STORY\\look\\%d\\look.txt", input.gender);
        fp = fopen(path, "r");
        if (fp)
        {
            // �����ȡ����ϴ��ǩ
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

    // // ���������Ϣ
    if (strstr(str, "<name>") || strstr(str, "<name_describe>"))
    {
        int name_gender;
        name_gender = rand() % 2;
        sprintf(path, "STORY\\name\\%c\\%d\\name.txt", input.genre, name_gender);
        fp = fopen(path, "r");
        if (fp)
        {
            // ��ȡ���������ϱ�ǩ
            int max_line;
            max_line = (name_gender == 0) ? 100 : 60;
            target_line = rand() % max_line + 1;
            delay(100);

            for (i = 1; fgets(buffer, MAX_CONTENT, fp); i++)
            {
                if (i == target_line)
                {
                    // ��������<name>������<name_describe>��������ѧ��...
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
            replaceTag(str, "<name>", "ĳ��");
            replaceTag(str, "<name_describe>", "");
            // break;
        }
    }

    // ����<location>,<pos_describe>
    if (strstr(str, "<location>") || strstr(str, "<pos_describe>"))
    {
        sprintf(path, "STORY\\location\\%c\\location.txt", input.genre);
        fp = fopen(path, "r");
        if (fp)
        {
            // ��ȡ���������ϱ�ǩ
            target_line = rand() % 50 + 1;
            delay(100);

            for (i = 1; fgets(buffer, MAX_CONTENT, fp); i++)
            {
                if (i == target_line)
                {
                    // ��������<location>�ռ�վ<pos_describe>�ռ��е�һĨ����...
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
            replaceTag(str, "<name>", "ĳ��");
            replaceTag(str, "<name_describe>", "");
        }
    }

    // ��������ռλ��
    while (strstr(str, "<words_good>") || strstr(str, "<words_bad>") ||
           strstr(str, "<think_good>") || strstr(str, "<think_bad>"))
    {
        // ����words·��
        sprintf(path, "STORY\\words\\%s\\words.txt", input.personality);
        fp = fopen(path, "r");
        if (fp)
        {
            // ȷ��������Χ
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
            // �ļ���ʧ�ܣ��滻Ĭ��ֵ
            replaceTag(str, "<words_good>", "\"���ͣ�\"");
            replaceTag(str, "<words_bad>", "\"��⣡\"");
            break;
        }
    }
}

// ͨ�ñ�ǩ������ȡ����
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

    // ���ι��˻��з�
    output[strcspn(output, "\r\n")] = 0;
}

// ��ǩ�滻����
void replaceTag(char *dest, const char *tag, const char *value)
{
    static char temp[MAX_CONTENT * 2] = {0}; // ˫�������ֹ���
    char *pos = NULL;
    int tagLen = strlen(tag);
    int valLen = strlen(value);

    while ((pos = strstr(dest, tag)) != NULL)
    {
        // �����滻�󳤶�
        size_t frontLen = pos - dest;
        size_t totalLen = frontLen + valLen + strlen(pos + tagLen);

        if (totalLen >= MAX_CONTENT)
        {
            break; // ��ֹ���������
        }

        // ������ʱ�ַ���
        strncpy(temp, dest, frontLen);
        temp[frontLen] = '\0'; // ȷ����ֹ
        strncat(temp, value, sizeof(temp) - strlen(temp) - 1);
        strncat(temp, pos + tagLen, sizeof(temp) - strlen(temp) - 1);

        // д��ԭ������
        strncpy(dest, temp, MAX_CONTENT - 1);
        dest[MAX_CONTENT - 1] = '\0';

        // �ƶ�ָ���������
        pos = dest + frontLen + valLen;
    }
}

// �������ɺ���
int storyGen(storyInput input)
{
    int story_num;
    srand(time(NULL));
    reset_cursor();
    // ���ɹ���
    story_num = generateStory(input);
    cleardevice();
    show_story();

    return story_num;
}