1) На стороне клиента проверять Количество 
   средств на балансе в вызове удалённого
   метода. ``` If sum > balance вызываем метод 
   else недостаточно средств. Работай ```  
   

* По клиенту, он будет отправлять на сервер
свой логин и пароль(хэш), (Авторизации нету)
  после чего сервер бужет возвращать статус
  Даётся три попытки, иначе sleep на 15 минут
  
* После входа можно использовать обычный
switch-case
  
* Майнер - тоже клиент, за каждую транзакцию
ему + токен
  