# OS_Project
 
## Đây là repo chung của bộ môn OS, hãy đọc kỹ nguyên tắc sử dụng

* Hiện tại chỉ có mới có một project, sau này khi có project khác, chúng ta vẫn sẽ submit và clone file trên repo này luôn. Mọi người hãy clone repo này về, và tạo một project bằng visual studio như đường dẫn bên dưới : 
```
OS_Project
	|--- OS_Project
		|--- README.md
		|--- .gitattributes
		|--- Project1(Tạo bằng VS cái này)
		|--- (Project2 Sau khi có Project 2 sẽ tạo bằng VS tiếp tục)
		|--- (...)
```
* Mỗi người đều có một branch riêng, đừng vội vàng *commit* lên branch **master** mà thay và đó hãy tự up lên branch của mình, thảo luận với người khác, kiểm tra bug rồi hãy *merge* với branch **master**
* Trước khi build, cần install thư viện **readline** bằng dòng lệnh 
``` sudo apt-get install libreadline-dev ```
* Build project sử dụng lệnh
``` gcc -Wall shell.c -lreadline -o shell ```
## Các liên kết ngoài
[Video](https://www.youtube.com/watch?v=17rJiKRD7mg)
[Instruction](https://nsl.cs.sfu.ca/teaching/11/300/prj2_shell.html)
