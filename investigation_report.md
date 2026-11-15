# מבצע B7 — דוח חקירה

**אל:** שירות הביטחון הכללי (שב״כ) ועיריית באר שבע  
**סטודנט/ית 1 — שם מלא:** Shira Finzi  
**מספר זהות:** 325047728  
**סטודנט/ית 2 — שם מלא (אם יש):** _Nimrod Jakobi______  
**מספר זהות (אם יש):** __207298522____  
**תאריך:** ____________________

> זהו דוח פרטי המוגש לצוות הקורס. מספרי הזהות משמשים לאימות קודי הגישה האישיים, ומותר לכלול אותם רק בדוח זה — לא בקוד המקור, בקובצי הראיות או בפלט התוכניות.
>
> ענו **בקצרה ובמילים שלכם**. אין צורך לתאר מחדש את הקוד או להעתיק את הוראות המשימה; התמקדו במה שגיליתם ובהחלטות הטכניות החשובות. אין להדביק פלט תוכנית מלא או צילומי מסך. תשובה קצרה ומנומקת מספיקה, ואין דרישת מינימום לאורך.
>
> מלאו כל פרק **רק לאחר השלמת המשימה המתאימה והרצתה בהצלחה**, כדי להסתמך על הממצאים שנחשפו בפועל.

## משימה 1 — השידור המיורט

### ממצאים

מה גיליתם בתשדורות, ומהו הממצא החשוב ביותר להמשך החקירה? ציינו את קוד הגישה בפורמט `B7-M1-XXXXXXXX`.

**תשובת הצוות:**
מצאנו מהתשדורות שהמבצע "operation Beershevanator" התחיל, והממצא החשוב הוא שיש אי התאמה בין ספירת הסוכנים לדוח, ויש סוכן "Agent 67" שמסתירים אותו במכוון.
קוד הגישה הוא `B7-M1-IIVMFCLF` 

### הבנה טכנית

הביאו **דוגמה אחת** לרשומה פגומה (אמיתית מהראיות או דוגמה שיצרתם), והסבירו מה גורם ל־`process_transmission` לדחות אותה. אין צורך לתאר את אלגוריתם הפענוח כולו.

**תשובת הצוות:**
רשומה פגומה מהראיות היא  - CONTROL|20|03|KUWA_IQN_NLUJ_YXIW_MMYWWU
מבנה תקין - שלושה '|'
נכשל על בדיקת checksum - בחישוב checksum עבור הרשומה יוצא 2 בעוד שברשומה רשום 3

---

## משימה 2 — הסוכן שלא קיים

### ממצאים

אילו אירועי גישה חריגים גיליתם, ומה אפשר להסיק מהם על סוכן 67? ציינו את קוד הגישה בפורמט `B7-M2-XXXXXXXX`.

**תשובת הצוות:**

<!-- כתבו כאן. -->

### הבנה טכנית

1. מדוע `find_agent` מחזירה מצביע לאיבר המקורי במערך ולא כתובת של משתנה מקומי?
2. מדוע אפשר לחשב את מספר האיברים במערך `working` באמצעות `sizeof` מתוך `main.c`, אבל לא מתוך הפרמטר `dest` של `copy_agents`?

**תשובת הצוות:**

<!-- כתבו כאן. -->

---

## משימה 3 — הגיבוי שפלנקטון שכח

### ממצאים

מה למדתם מההתכתבויות ששוחזרו, וכיצד הן משפיעות על המשך המבצע? ציינו את קוד ההשלמה בפורמט `B7-M3-XXXXXXXX`.

**תשובת הצוות:**

from the recovered communications we found that the Beershevanator is planned to disrupt traffic control, access systems, and city infrastructure, and itll activate at midnight. Also theres a self destruct button we can activate from the server. also morty and agent 67 have a private comm channel that others dont know about. 
code is B7-M3-OAYTIOFD

### הבנה טכנית

1. הבחינו בין **קלט מושאל** לבין זיכרון שבבעלות `MessageStore`: מה מותר לשחרר ומי אחראי לכך?
2. נניח שהקצאת זיכרון נכשלת באמצע `store_clone`. מה חייב לקרות להקצאות שכבר בוצעו ולמצב של `dest`, ומדוע?

**תשובת הצוות:**

1) Borrowed input is text passed to a function that belongs to someone else. func can read and copy it, but cant free it. MessageStore owns its array of messages and copies (deep) of their text. So, it is responsible for freeing both the array, and the copies of the text when they are no longer needed. the original text, is not its respoibsility, but of the og owner.
2) if memory allocation fails in the middle of store_clone, we must call free_store on the dest in order to free all the mem already allocated to prev messages thta were cloned. this prevents memory leaks and leaves a dest that is empty. 
---

## משימה 4 — הבקר השבור של פלנקטון

### ממצאים

מה הצלחתם לשחזר בבקר, ומה עדיין מונע את השלמת הפעולה? ציינו את קוד ההשלמה בפורמט `B7-M4-XXXXXXXX`.

**תשובת הצוות:**
 we recovered the command categoreis: OPERATIONS, DIAGNOSTICS, and EMERGENCY_CONTROL.
 we found the emergency component at index 0, with ID 102. its called EMERGENCY_CORE, and its role is SELF_DESTRUCT_INTERFACE.
 we also found the emergency workflow: 
 1. PREPARE_EMERGENCY
2. REQUEST_AUTHORIZATION
3. EXECUTE_COMMAND

Access code: B7-M4-OOESLCNY
 
### תיעוד התיקונים

**לכל אחד מהשלבים A–D**, תעדו בקצרה: מה היה שורש התקלה, מה שיניתם ומדוע התיקון עובד, ואיזו בדיקה מאמתת אותו (קלט או תרחיש ותוצאה צפויה). מספיקות כמה שורות לכל שלב.

**שלב A:**

The code allowed length == capacity, even though we also need space for '\0'. We changed > to >= to prevent writing outside the buffer.
Test: command with length 2 (ex. "GO") and capacity 2 should fail. but length 2 and capacity 3 should pss

**שלב B:**

The code allowed index == count, which is out of bounds because indexing starts at 0. We changed > to >=.
Test: an array with 3 elements, index 2 is valid but index 3 should be invalid and return null

**שלב C:**

memmove() expects bytes, but we were passing the number of remaining elements. We fixed it by multiplying by sizeof(ControlCommand).
Test: Removing B from [A, B, C, D] leaves [A, C, D], count=3 and one unsuded slot 

**שלב D:**

The code freed the command's text immediately after scheduling it, even though we still needed it. We removed that free() so the text remains available until execution.
Test: schedualing command SELF_DESTRUCT_PREPARE should preserve the command, and memory should only be freed once the command is exectuted 

### חקירת GDB אחת

בחרו **שלב אחד בלבד** מבין A–D. היכן עצרתם, באילו פקודות עיקריות השתמשתם, מה ראיתם בפועל וכיצד התצפית עזרה לזהות את התקלה? אין צורך בתמליל או בצילום מסך.

**תשובת הצוות:**

We investigated controller_parse_command() by setting a breakpoint at the function and examining its arguments and the calculated string length using info args, next, and print.
The problematic condition was length > capacity. When the string length equals the buffer capacity, the function incorrectly allows the copy, even though an additional byte is required for the null terminator.
---

## משימה 5 — הפקודה האחרונה

### ממצאים

מה גיליתם על תהליך ההרשאה, מה הייתה תוצאת המבצע ומה נשאר לא ידוע בסיום החקירה? ציינו את קוד ההשלמה בפורמט `B7-M5-XXXXXXXX`. התבססו רק על המידע שנחשף בהרצה.

**תשובת הצוות:**

<!-- כתבו כאן. -->

### הבנה טכנית

1. מה ההבדל בין בקשת חירום שממתינה לאישור לבין בקשה שאושרה, ומדוע לא מספיק לשנות את שדה האישור ידנית?
2. אילו נתונים בבקשה חייבים להישמר בעותק עצמאי, ומה עלול לקרות אם נשמרים רק מצביעים למאגר הראיות?
3. תארו **מקרה כשל אחד** שבו אסור לשלוח את פקודת הביצוע, והסבירו מה בקוד מונע זאת.

**תשובת הצוות:**

1) a pending request has approved = 0, while an approved request has approved = 1. We cant just change it manually because the approval needs to come from the authorization service. 
2) we need independent copies of device and steps, since the original data might be changed or freed. Otherwise, our request could end up pointing to invalid memory. We also copy the authorization struct by value and the other strings into the requests arrays.

