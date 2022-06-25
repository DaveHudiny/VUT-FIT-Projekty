<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

class CreateAuctionsTable extends Migration
{
    /**
     * Run the migrations.
     *
     * @return void
     */
    public function up()
    {
        Schema::create('auctions', function (Blueprint $table) {
            $table->id();
            $table->unsignedBigInteger('seller_id');
            $table->foreignId('licitator_id');
            $table->foreignId('winner_id');
            $table->boolean('approved');
            $table->string('name');
            $table->text('description');
            $table->string('type');
            $table->integer('starting_price');
            $table->integer('minimum_wager');
            $table->text('requirements')->nullable(1);
            $table->date('date');
            $table->boolean('is_open');
            $table->boolean('ended');
            $table->boolean('confirmed')->nullable(1);
            $table->timestamps();

            $table->foreign('seller_id')->references('id')->on('users')->cascadeOnDelete();
            //$table->foreign('licitator_id')->references('id')->on('users')->cascadeOnDelete();
        });
    }

    /**
     * Reverse the migrations.
     *
     * @return void
     */
    public function down()
    {
        Schema::dropIfExists('auctions');
    }
}
