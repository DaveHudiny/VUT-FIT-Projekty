<x-layout>  
    <br>
    <h1 class="ml-3 text-2xl font-bold text-black">Licitator page</h1>
    <br>
        
    <h2 class="ml-5 text-xl font-bold text-black">Unapproved auctions</h2>
    <div style="overflow-x:auto;">
        <table id="users">
            <tr>
                <th>Auction ID</th>
                <th>Name of auction</th>
                <th>Name</th>
                <th>Username</th>
                <th>Finish date</th>
                <th>Starting price</th>
                <th>Minimal wager</th>
                <th>Type</th>
                <th>Approve</th>
                <th>Refuse</th>
            </tr>
            @foreach($auctionUnapproved as $auction)
                @if(Auth::user()->id == $auction->seller_id)
                    @continue
                @endif
                <form action="/licitpage" method=POST>
                    <input type="text" name="auctionID" value={{ $auction->id }} hidden/>
                    <input type="text" name="licitatorID" value={{ Auth::user()->id }} hidden/>
                    @csrf
                    <tr>
                        <td><a href="/auction/{{ $auction->id }}">{{ $auction->id }}</a></td>
                        <td>{{ $auction->name }}</td>
                        <td>{{ $auction->user->name }}</td>
                        <td><a href="/users/{{ $auction->user->id }}">{{ $auction->user->username }}</a></td>
                        <td>{{ \Carbon\Carbon::parse($auction->date)->format('j F, Y') }}</td>
                        <td>{{ $auction->starting_price }} €</td>
                        <td>{{ $auction->minimum_wager }} €</td>
                        <td>
                            @if($auction->type=="demand")
                                Demand
                            @else
                                Offer
                            @endif
                        </td>
                        <td><input type="submit" value="Approve" name="action" onclick="return confirm('Approve auction?')"></td>
                        <td><input type="submit" value="Refuse" name="action" onclick="return confirm('Delete acution? This action is not reversible.')"></td>
                    </tr>
                </form>
            @endforeach
        </table>
    </div>

    <h2 class="ml-5 text-xl font-bold text-black">Approved auctions (you licitate)</h2>

    <div style="overflow-x:auto;">
        <table id="users">
            <tr>
                <th>Auction ID</th>
                <th>Name of auction</th>
                <th>Name</th>
                <th>Username</th>
                <th>Finish date</th>
                <th>Starting price</th>
                <th>Minimal wager</th>
                <th>Offers</th>
                <th>Delete auction</th>
            </tr>
            @foreach($auctionApproved as $auction)
                @if(Auth::user()->id == $auction->seller_id)
                    @continue
                @endif
                <form action="/licitpage" method=POST>
                    <input type="text" name="auctionID" value={{ $auction->id }} hidden/>
                    @csrf
                    <tr>
                        <td><a href="/auction/{{ $auction->id }}">{{ $auction->id }}</a></td>
                        <td>{{ $auction->name }}</td>
                        <td>{{ $auction->user->name }}</td>
                        <td><a href="/users/{{ $auction->user->id }}">{{ $auction->user->username }}</a></td>
                        <td>{{ \Carbon\Carbon::parse($auction->date)->format('j F, Y') }}</td>
                        <td>{{ $auction->starting_price }} €</td>
                        <td>{{ $auction->minimum_wager }} €</td>
                        <td><a href="/users/{{ Auth::user()->id }}/auctions">Administrate offers here</a>
                        <td><input type="submit" value="Delete" name="action" onclick="return confirm('Delete acution? This should be done only when the auction is illegal or something like that.')"></td>
                    </tr>
                </form>
            @endforeach
        </table>
    </div>
    <h2 class="ml-5 text-xl font-bold text-black">Finished auctions</h2>
    <div style="overflow-x:auto;">
        <table id="users">
            <tr>
                <th>Auction ID</th>
                <th>Name of auction</th>
                <th>Name</th>
                <th>Username</th>
                <th>Finish date</th>
                <th>Final price</th>
                <th>Winner</th>
                <th>Declare results</th>
            </tr>
            @foreach($auctionFinished as $auction)
                @if(Auth::user()->id == $auction->seller_id)
                    @continue
                @endif
                <form action="/licitpage" method=POST>
                    <input type="text" name="auctionID" value={{ $auction->id }} hidden/>
                    @csrf
                    <tr>
                        <td><a href="/auction/{{ $auction->id }}">{{ $auction->id }}</a></td>
                        <td>{{ $auction->name }}</td>
                        <td>{{ $auction->user->name }}</td>
                        <td><a href="/users/{{ $auction->user->id }}">{{ $auction->user->username }}</a></td>
                        <td>{{ \Carbon\Carbon::parse($auction->date)->format('j F, Y') }}</td>
                        <td>{{ $auction->starting_price }} €</td>
                        <?php
                            $offers = $auction->offer->where('approved', true);
                            if($auction->type="demand")
                            {
                                $offer = $offers->where("value", "=", $offers->min("value"));
                            }
                            else 
                            {
                                $offer = $offers->where("value", "=", $offers->max("value"));
                            }
                            if($offer->first() != null)
                            {
                                $offer=$offer->first;
                                $id = $offer->user->id;
                                $username = $offer->user->username;
                                $value = $offer->value;
                                echo "<td><a href=/users/$id>$username</a></td><td>$value €</td>";
                            }
                            else
                            {
                                echo "<td>0 €</td><td>Nobody contested</td>";
                            }
                        ?>
                        <td><form method=POST action="/licitpage">
                            <input type=text hidden name=auctionID value={{$auction->id}}>
                            <input type="submit" name="action" value="Confirm">
                        </form>
                        </td>
                    </tr>
                </form>
            @endforeach
        </table>
    </div>
</x-layout>