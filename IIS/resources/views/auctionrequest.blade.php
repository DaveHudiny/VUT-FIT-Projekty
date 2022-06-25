<x-layout>
    <br>
    <h1 class="ml-10 text-2xl font-bold text-black">Create auction</h1>
    <div id="wrapper" >
        <form action="/auction/request/#" method=post enctype="multipart/form-data" class="lg:flex justify-center bg-blue-100 rounded-lg">
            <div class="lg:pr-14">
                @csrf
                <input hidden name="id" value={{Auth::user()->id}} class="lg:w-96">
                <label>Name*</label>
                <input name="name" type=text value="{{old('name', '')}}" class="lg:w-96">
                <label>Description*</label>
                <!-- <input name="desc" type=text value="{{old('desc', '')}}"> -->
                <textarea rows="5" type="text" name="desc" value="{{old('desc', '')}}" class="lg:w-96"></textarea>
                <label>Type*</label>
                <select name="type">
                    @if (old('type') == 'demand')
                        <option value="demand" selected>Demand</option>
                    @else
                        <option value="demand">Demand</option>
                    @endif
                    @if (old('type') == 'offer')
                        <option value="offer" selected>Offer</option>
                    @else
                        <option value="offer">Offer</option>
                    @endif
                </select>
                <br>
                <br>
                <label>Starting price [€]*</label>
                <input type="number" name="price" value="{{old('price', '')}}" class="lg:w-96">
                <br>
                <label>Minimum wager [€]*</label>
                <input type="number" name="minimum_wager" class="lg:w-96">

                <label>Rules*</label>
                <select name="rules">
                    @if (old('rules') == 'open')
                        <option value="open" selected>Open</option>
                    @else
                        <option value="open">Open</option>
                    @endif
                    @if (old('rules') == 'closed')
                        <option value="closed" selected>Closed</option>
                    @else
                        <option value="closed">Closed</option>
                    @endif  
                </select>
            </div>
            <div class="lg:pl-14">
                <label>Special requirements</label>
                <!-- <input name="requirements" type=text value="{{old('requirements', '')}}"> -->
                <textarea rows="5" type="text" name="requirements" value="{{old('requirements', '')}}"></textarea>
                <label>End of auction (auction ends 23:59:59 at chosen date)</label>
                <input type="date" name="date" value={{ Carbon\Carbon::now()->toDateTimeString() }}>
                <label>Image</label>
                <input type="file" name="myphoto" accept="image/*">
                <input value="Send request" type="submit" id="btn_send">
                <h6 style="color: red;font-size:small;margin-top:2px;">* required option<h6>
                @if ($errors->any())
                    <div class="alert alert-danger text-red-500">
                        <ul>
                            @foreach ($errors->all() as $error)
                                <li>{{ $error }}</li>
                            @endforeach
                        </ul>
                    </div>
                @endif
            </div>
        </form>
    </div>
</x-layout>