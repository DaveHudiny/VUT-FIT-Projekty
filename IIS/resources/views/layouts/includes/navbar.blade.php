@if (Route::has('login'))
    <nav>
        <div class="flex justify-between items-center">
            <div>
                <a href="{{ url('/') }}" class="text-2xl xl:text-3xl text-black px-3 font-bold flex items-center">
                    <img src="/images/icon_black.png" alt="Auction House logo" style="width:45px; height:auto;"><p class="text-center">&nbsp; Auction House</p>
                </a>
            </div>
            <!-- Desktop -->
            <div class="hidden lg:flex items-center">
                @auth
                    <a href="{{ url('/') }}" class="text-sm font-bold text-black mx-3 text-center">Main page</a>
                    <a href="/auction/request" class="text-sm font-bold text-black mx-3 text-center">Request Auction</a>
                    <a href="/users/{{Auth::user()->id}}" class="text-sm font-bold text-black mx-3 text-center">{{Auth::user()->username}}</a>
                    <a href="/users/{{Auth::user()->id}}/auctions" class="text-sm font-bold text-black mx-3 text-center">My auctions</a>
                    @if (Auth::user()->username=="Admin")
                        <a href="/admin/users" class="text-sm font-bold text-black mx-3 text-center">User administration</a>
                    @endif
                    @if (Auth::user()->licitator)
                        <a href="/licitpage" class="text-sm font-bold text-black mx-3 text-center">Licitator page</a>
                    @endif
                        <form class="bg-black rounded-full text-lg font-bold text-white px-4 mx-4 text-center" method="POST" action="{{ route('logout') }}">
                            @csrf
                            <button type="submit">Log Out</button>
                        </form>
                @else
                    <a href="{{ route('login') }}" class="text-lg font-bold text-black mx-3">Log in</a>

                    @if (Route::has('register'))
                        <a href="{{ route('register') }}" class="text-lg font-bold text-black ml-3 mr-6">Register</a>
                    @endif
                @endauth
            </div>
            <!-- End Desktop -->
            <!-- Mobile -->
            <div class="lg:hidden flex items-center">
                <button class="mobile-menu-btn mr-4">
                    <svg class="w-6 h-6" xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M4 6h16M4 12h16M4 18h16" />
                    </svg>
                </button>
            </div>
        </div>
        <!-- Horizontal line --> <hr style="height:2px;border-width:0;color:black;background-color:black;">
        <div class="hidden mobile-menu lg:hidden">
            @auth
                <a href="{{ url('/') }}" class="text-sm font-bold text-black py-2 px-4 block hover:white">Main page</a>
                <a href="/auction/request" class="text-sm font-bold text-black py-2 px-4 block hover:white">Request Auction</a>
                <a href="/users/{{Auth::user()->id}}" class="text-sm font-bold text-black py-2 px-4 block hover:white">{{Auth::user()->username}}</a>
                <a href="/users/{{Auth::user()->id}}/auctions" class="text-sm font-bold text-black py-2 px-4 block hover:white">My auctions</a>
                @if (Auth::user()->username=="Admin")
                    <a href="/admin/users" class="text-sm font-bold text-black py-2 px-4 block hover:white">User administration</a>
                @endif
                @if (Auth::user()->licitator)
                    <a href="/licitpage" class="text-sm font-bold text-black py-2 px-4 block hover:white">Licitator page</a>
                @endif
                    <form class="text-lg font-bold text-black py-2 px-4 block hover:white" method="POST" action="{{ route('logout') }}">
                        @csrf
                        <button type="submit">Log Out</button>
                    </form>
            @else
                <a href="{{ route('login') }}" class="text-lg font-bold text-black py-2 px-4 block hover:white">Log in</a>

                @if (Route::has('register'))
                    <a href="{{ route('register') }}" class="text-lg font-bold text-black py-2 px-4 block hover:white">Register</a>
                @endif
            @endauth
            <script>
                const btn = document.querySelector('button.mobile-menu-btn');
                const menu = document.querySelector('.mobile-menu');

                btn.addEventListener("click", () => {
                    menu.classList.toggle("hidden");
                })
            </script>
        </div>
        <!-- End Mobile -->
    </nav>
@endif

<!-- @if (Route::has('login'))
    <nav class="flex justify-between items-center">
        <div>
            <a href="{{ url('/') }}" class="text-3xl text-black px-5 font-bold flex items-center">
                <img src="/images/icon_black.png" alt="Auction House logo" style="width:45px; height:auto;"><p> &nbsp; Auction House</p>
            </a>
        </div>
        <div class="flex items-center">
            @auth
                <a href="{{ url('/') }}" class="text-sm font-bold text-black mx-3">Main page</a>
                <a href="/auction/request" class="text-sm font-bold text-black mx-3">Request Auction</a>
                <a href="/users/{{Auth::user()->id}}" class="text-sm font-bold text-black mx-3">{{Auth::user()->username}}</a>
                <a href="/users/{{Auth::user()->id}}/auctions" class="text-sm font-bold text-black mx-3">My auctions</a>
                @if (Auth::user()->username=="Admin")
                    <a href="/admin/users" class="text-sm font-bold text-black mx-3">User administration</a>
                @endif
                @if (Auth::user()->licitator)
                    <a href="/licitpage" class="text-sm font-bold text-black mx-3">Licitator page</a>
                @endif
                    <form class="bg-black rounded-full text-lg font-bold text-white px-4 mx-4" method="POST" action="{{ route('logout') }}">
                        @csrf
                        <button type="submit">Log Out</button>
                    </form>
            @else
                <a href="{{ route('login') }}" class="text-lg font-bold text-black mx-3">Log in</a>

                @if (Route::has('register'))
                    <a href="{{ route('register') }}" class="text-lg font-bold text-black ml-3 mr-6">Register</a>
                @endif
            @endauth

        </div>
    </nav>
    <hr style="height:2px;border-width:0;color:black;background-color:black;">
@endif -->