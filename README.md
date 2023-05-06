# Fish Eat Fish
## Giới thiệu
* Đây là một trò chơi được viết bằng ngôn ngữ ``C++`` sử dụng thư viện ``SDL2``.
* Trò chơi có tên ``Fish eat fish`` hoặc ``Cá lớn nuốt cá bé``. 
## Cốt truyện
* Bạn trong vai một chú cá đèn lồng sống trong đại dương mênh mông, cố gắng tìm kiếm thức ăn để tổn tại và phát triển. 
* Trong đại dương có rất nhiều mối nguy hiểm, bao gồm các loài cá ăn thịt lớn hơn và những quả bom hạt nhân. Hãy vượt qua các mối nguy hiểm và vươn lên đứng đầu chuỗi thức ăn. 

## Cài đặt

* Bước một, cài đặt [g++](https://formulae.brew.sh/formula/gcc#default) sử dụng Homebrew
* Bước thứ hai, cài đặt [SDL2](https://github.com/rosejoshua/QuickSDL2Mac), [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer/releases), [SDL2_image](https://github.com/libsdl-org/SDL_image/releases/tag/release-2.6.3), [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.20.2). Để các thư viện này trong thư mục có tên ``SDL2.framework``.  Hoặc có thể tải sẵn thư viện của tôi tại [đây](https://drive.google.com/file/d/10YlzZuQPUc5oR2w4aWxxQ1EQC_B761Xt/view?usp=sharing). Để thư mục này tại ``/Library/Frameworks``.
* Bước thứ ba, clone repo này về máy của bạn. Chuyển hướng sang thư mục ``src``. Mở ``cmd`` và gõ ``chmod u+x comprun.sh``, sau đó gõ ``./comprun.sh``. Màn hình game sẽ xuất hiện.
* Giờ hãy thưởng thức trò chơi..! 😋

## Cách chơi
### Các nút bấm
* ``New game``: bắt đầu màn chơi mới.
* ``High Score``: danh sách người chơi có điểm cao.
### Điều khiển
* Sử dụng chuột để điều khiển chú cá của bạn. Bạn chỉ có thể ăn những con cá có điểm nhỏ hơn hoặc bằng điểm số của bạn hiện tại. Điểm số sẽ được hiển thị ở phía bên trên màn hình.  
* Ấn phím ``v`` để bật / tắt âm thanh.
* Ấn phím ``Space`` để dừng / tiếp tục trò chơi.
