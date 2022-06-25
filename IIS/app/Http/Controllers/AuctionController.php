<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use App\Models\User;
use App\Models\Auction;
use Carbon\Carbon;

class AuctionController extends Controller
{
    public function index(){
        
        $auctions = Auction::where([['approved', true], ['date', '>=', Carbon::now()->toDateString()],
        ['confirmed', false]]);
        // if(request('type')){
        //     if(request('type' == 'demand')){
        //         $auctions->where('type', 'demand');
        //     }
        // }
        
        $auctions = $auctions->filter(request(['search','type','rule']));
        return view('welcome', [
            'auctions' => $auctions->paginate(20),
            'currentType' => request('type'),
            'currentRule' => request('rule')
        ]);

    }
}